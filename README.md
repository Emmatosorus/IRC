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
Server is compatible with [HexChat](https://hexchat.github.io/), which is a proper GUI IRC client.

Another way to connect is from the command line: use `nc`: `sed -u 's/$/\r/g' | nc <Server IP> <Server Port>`. This is the most useful during testing.
Not all `nc` implementations support automatic `\r\n` insertion at the end of the message, but one can use `sed` to circumvent that.

### Syntax
 - `:` Everything after will be considered one argument (including spaces)
 - `,` If words are spaced only with commas it will be considered one list argument

#### Commands
##### Connection & Authentication
- `PASS <password>` : Authenticates with the server (must be entered first on connection)
- `NICK <nickname>` : Sets your nickname (required on connection)
- `USER <username> 0 * <realname>` : Sets up your user profile (required on connection)
- `PING <token>` : Server keep-alive, responds with PONG
- `QUIT [<reason>]` : Disconnects from the server with optional reason
- `AWAY [<message>]` : Sets or removes your away status

##### Channel Operations
- `JOIN <channel>{,<channel>} [<key>{,<key>}]` : Joins one or more channels, key is required if the channel has a password
- `PART <channel>{,<channel>} [<reason>]` : Leaves one or more channels with optional reason
- `LIST` : Shows the list of channels with information about them
- `NAMES [<channel>]` : Shows the list of users in a channel
- `TOPIC <channel> [<topic>]` : Views or sets the channel topic
- `KICK <channel> <user> [<reason>]` : Removes a user from a channel (operator only)
- `INVITE <nickname> <channel>` : Invites a user to a channel

##### Messaging
- `PRIVMSG <target>{,<target>} :<message>` : Sends a message to one or more users or channels
- `NOTICE <target>{,<target>} :<message>` : Sends a notice message (non-auto-reply)

##### User Commands
- `MOTD` : Displays the message of the day

##### Channel Modes (MODE command)
- `MODE <channel> [<modes> [<arguments>]]` : Views or changes channel modes (operator only for changes)
  - `+i/-i` : Invite-only mode (users must be invited to join)
  - `+t/-t` : Topic restriction (only operators can change the topic)
  - `+n/-n` : No external messages (must be in channel to send messages)
  - `+k/-k <password>` : Set/remove channel password
  - `+l/-l <limit>` : Set/remove user limit on channel
  - `+o/-o <nickname>` : Grant/remove operator privileges to a user

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
