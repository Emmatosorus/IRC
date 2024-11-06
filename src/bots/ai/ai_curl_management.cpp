/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_curl_management.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:00 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 12:59:02 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

/*
The ai uses the api of chatgpt.
To call it, the easiest method is to use a curl command.
To do that, we need to send the curl command in the terminal.
With the help of the popen function, which sends the curl into the terminal and do a redirection
into a file, we can call the ai and then access her answer. Of course this is easier said than done,
there is small things to understand to make this process smooth. Each functions here is a step.
*/

// First, clean up the message of quotes, which breaks the curl function otherwise.

int Ai::cleanup_msg()
{
    size_t pos = msg.find_first_of("\'\"", 0);

    while (pos != std::string::npos)
    {
        msg.erase(pos, 1);
        msg.insert(pos, " ");
        pos = msg.find_first_of("\'\"", pos + 1);
    }
    pos = msg.find("\r\n", 0);
    if (pos == std::string::npos)
        return (error_msg("\033[0;31mError! Client msg has an error!\033[0m", 1));
    msg.erase(pos, 2);
    return (SUCCESS);
}

// Second, put the message in the correct section of the curl command (the second content is to fill
// up with the actual message)

int Ai::setup_curl_cmd()
{
    size_t pos;
    size_t end_pos;

    pos = curl_cmd.find("content", 0);
    if (pos == std::string::npos)
        return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));

    pos = curl_cmd.find("content", pos + 7);
    if (pos == std::string::npos)
        return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));

    pos += 11;
    end_pos = curl_cmd.find("\"", pos);
    if (end_pos == std::string::npos)
        return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));

    curl_cmd.erase(pos, end_pos - pos);
    curl_cmd.insert(pos, msg);
    return (SUCCESS);
}

// Third, we need to execute the command, find any potential errors and get the correct line with
// the ai's message

int Ai::execute_curl_cmd_and_parse_result()
{
    FILE* fp = popen(curl_cmd.c_str(), "r");
    char fgets_buf[1024];
    int status;
    size_t pos = 0;

    if (fp == NULL)
        return (error_msg("\033[0;31mError! Popen error!\033[0m", 1));

    while (fgets(fgets_buf, 1024, fp) != NULL)
    {
        ai_msg = fgets_buf;
        pos = ai_msg.find("content", 0);
        if (ai_msg.find("You didn't provide an API key.", 0) != std::string::npos ||
            ai_msg.find("Incorrect API key provided", 0) != std::string::npos)
            return (pclose(fp), error_msg("\033[0;31mError! API key error!\033[0m", 1));
        if (ai_msg.find("We could not parse the JSON body of your request.", 0) !=
            std::string::npos)
            return (pclose(fp), error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
        if (pos != std::string::npos)
            break;
    }
    if (pos == 0)
        return (
            pclose(fp),
            error_msg("\033[0;31mError! You did a redirection again you silly goose!\033[0m", 1));

    status = pclose(fp);
    if (status == -1)
        return (error_msg("\033[0;31mError! Pclose error!\033[0m", 1));

    return (SUCCESS);
}

// Finaly, we just need to get the message from the line we just got, and put it in the norm for an
// irc server (add PRIVMSG and the name of the sender to the ai message)

int Ai::find_and_prep_ai_msg_for_send()
{
    size_t pos = ai_msg.find("content", 0);
    size_t end_pos = 0;
    size_t tmp_pos = 0;

    if (pos == std::string::npos)
        return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
    pos += 11;
    end_pos = ai_msg.find("\"", pos);
    tmp_pos = end_pos;
    while (tmp_pos != std::string::npos)
    {
        end_pos = ai_msg.find("\"", tmp_pos + 1);
        tmp_pos = ai_msg.find("\"", end_pos + 1);
    }
    if (end_pos == std::string::npos)
        return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
    ai_msg[end_pos] = '\0';
    ai_msg = &ai_msg[pos];
    msg = "PRIVMSG " + sender_name + ": " + ai_msg + "\r\n";
    return (SUCCESS);
}