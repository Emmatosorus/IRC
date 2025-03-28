# IRC Server with AI & Moderation Bot 🚀

Welcome to our **IRC Server**, a chat server featuring two powerful bots:

- **AI Bot 🤖**: An AI-powered assistant that responds to users in a ✨fun✨ way.
- **Guardian Bot 🛡️**: Informs channel operators if a word stored in it's dictionnairy was mentionned.
  
## → What is IRC? 🤔
IRC (Internet Relay Chat) is a protocol for real-time text-based communication. It allows users to join channels (chat rooms) 
and communicate with others over a network. It's widely used for group discussions and support communities

## → Installation ⚙️
### Setup
**1 -** Clone the repository:
```sh
git clone https://github.com/Emmatosorus/IRC
```
**2 -** Build the server:
```sh
cd IRC/
make
```
**3.1 -** Build the AI Bot (OPTIONAL):
```sh
make ai_launch
```

**3.2 -** Build the Guardian Bot (OPTIONAL):
```sh
make guardian_launch
```
### Running the Server
```sh
./ircserv <Port> <Password>
```

### Running the Bots
#### ChatGPT Bot
To enable the AI bot, set up your OpenAI API key:
```sh
export OPENAI_API_KEY="your_api_key_here"
./ai <Server IP> <Server Port> <Server password> <Name (optionnal)>
```

#### Moderation Bot
Simply run:
```sh
./bot <Server IP> <Server Port> <Server password> <Name (optionnal)>
```

## Usage 📡
### The Server
To connect to the server, use an IRC client (e.g., HexChat or NetCat).

### Syntax
 - `:` Everything after will be considered one argument (including spaces)
 - `,` If words are spaced only with commas it will be considered one list argument

#### Commands
  - `NICK <nickname>` : Sets your nickname (must be entered on connection)
  - `USER <username> 0 * <realname>` : Sets up your user profile (must be entered on connection)
  - `PASS <password>` : Enters password to enter the server (must be entered after the two commands above)
  - `PRIVMSG <target>{,<target>} <text to be sent>` : Will send a message to the recepiente.s
  - `LIST` : Will show you the list of channels with some information about them
  - `JOIN <channel>{,<channel>} [<key>{,<key>}]` : Let's you join a channel, key is necessary only if the the channel has a password
  - `PART <channel>{,<channel>} [<reason>]` : Lest's you leave a channel, reason is optional
  - **WIP**

### The AI Bot
You can talk with the AI bot through private messages.
## WIP

### The Guardian Bot
The moderation bot will keep an eye on inappropriate language.
## WIP

## Contributors 🤝
This project was made by [emuminov](https://github.com/emuminov), [Celiastral](https://github.com/Celiastral) 
and [Emmatosorus](https://github.com/Emmatosorus)

---

Enjoy chatting on your IRC Server! 🎉
