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
#### Commands
  # WIP


### The AI Bot
You can talk with the AI bot through private messages.

### The Guardian Bot
5. The moderation bot will keep an eye on inappropriate language.

## Contributors 🤝
This project was made by [emuminov](https://github.com/emuminov), [Celiastral](https://github.com/Celiastral) 
and [Emmatosorus](https://github.com/Emmatosorus)

---

Enjoy chatting on your IRC Server! 🎉
