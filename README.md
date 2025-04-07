## Why I created this repository

I created this Repository to review the content of memory safety

This week I got a chance of interview from a very big and advanced company,
and in this interview one of the interviewers has prepared a small task for me to implement in limited time

I have to confess I did not deliver a good performance in this interview (I want to apologyze to them for that,
cause this company treated me very friendly and I could see that they took this interview really seriously),
since I haven't touched safe casting topic for a very long time and have already forgotten some basic concepts,
I did not implement it fluently and could not answer some of the questions

In this repository I will implement a self defined protocol with c++ and socket to make a clear understanding of safe casting
(anyone who sees this, please read 2. Chapter of C++ Primer, it helps a lot about memory safety with different data types, please, please,
it is very very very very important for embedded system development)

Thanks to this company and to the interviewers and the presentation from Bjarne Stroustrup, thank everyone who let me know where I still need
to go deep into to complete my knowledge system

I don't know if someone from this company would see this, but all the best wishes to this company, a company with this high standard of development
deserves to be the leader of the market (and it already is) and anyone who can have a chance to work there is the luckiest guy in the world

## The message sequence is shown as below:
<picture style="padding-bottom: 1em;">
  <img alt="Shows Ankaios logo" src="message sequence.png">
</picture>

## Eplaination of protocol message (application layer)

## MessageType
| Name          | Value         |
| ------------- | ------------- |
| Unknown       | 0             |
| Connect       | 1             |
| Accept        | 2             |
| Error         | 3             |
| Store         | 4             |
| Load          | 5             |
| Value         | 6             |

## ConnecMessage
If the type of message is Connect, then this message is a ConnectMessage, Messages of this
type have no additional payload. Thus, the playload size if always 0. After connecting to the server's
TCP socket, a client shall send a message of this type to initiate the hand shake.
|               | Byte index range (inclusive)|Data type    |Description            |
| ------------- | ----------------------------|-------------|-----------------------|
| Header        | 0....0                      |MessageType  |The type of the message|
|               | 1....4                      |uint32       |The size of the payload in bytes|
| Accept        | 5....8                      |uint32       |The client id, this is a unique number assigned to the client by the server|
 
