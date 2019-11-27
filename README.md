1
# GuessHowMuch
A Qt console client-server random number guessing game

When the client connects to the server, the server chooses a random number for the user to guess.
Each time the user enters a guess, the server responds with an indication helping the user to guess.
The client can be limited to a maximum number of tries, and if this limit is reached, the user looses.
After each game, the user will be able to see his 5 best scores.

# How does it work

## Server

Launch the server using the following command :
```./server [-p|--port PORT] [-l|--limit LIMIT] [-b|--bounds LOWBOUND,HIGHBOUND]```

### Options
-p --port	PORT (default=4242)
  Specifies the port number on which the server should run.
  
-l --limit LIMIT (default=-1)
  Specifies the maximum number of tries a client will have to find the random number chosen by the server (-1=unlimited)
  
-b --bounds LOWBOUND,HIGHBOUND (default=1,100)
  Specifies the bounds between which the server will choose the number the client will have to find.
  Specified Bounds can be negative or positive numbers.
  Specified Bounds are included in the range of choosable numbers.

## Client

```./client [-h|--host HOSTNAME] [-p|--port PORT] [-n|--name NAME] [-a|--auto]```

### Options

-h --host	HOSTNAME (default=localhost)
  Specifies the server hostname.
  
-p --port	PORT (default=4242)
  Specifies the port on which the serve is runnning. 
  
-n --name	NAME (default="")
  Specifies the user's name.
  This option does nothing if the --auto option is present

-a --auto
  If specified, activates the automatic mode, which will play instead of the user.

# Compilation

//TODO:
