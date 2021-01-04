# **P2P File Transfer**

## **Overview**
* When log in, Make clients are sending their P2P IP/Port#
* If a special msg (e.g.,“[FILE]”) is received, a P2P file transfer process is starting
* P2P Server-Client is newly generated with P2P IP/Port#. (additional fork() calls are required.)
* In P2P(Peer To Peer) netwotk, there are no master-slave realtionship. 
  * So original communication(Server) need to be disconnected and direct communication between client1,2 should be established.  

## **Description**
* Design as model structure below
<img src="https://user-images.githubusercontent.com/48046183/103541137-47d85c00-4ede-11eb-9c90-a1e7c4dbbc81.png" width="90%">

## **Results**
* **User1(client1) request [FILE] - Server screen**
<img src="https://user-images.githubusercontent.com/48046183/103543300-9e936500-4ee1-11eb-962b-6b96938a206a.png" width="50%">

* **Change P2P network and Request 'a-2.txt' file that is third in file list - Client1 screen**
<img src="https://user-images.githubusercontent.com/48046183/103543652-3c872f80-4ee2-11eb-9096-b4ef0b494690.png" width="30%">

* **Change P2P netwrok with client1's IP/Port# and Send file list, requested file's name and content**
<img src="https://user-images.githubusercontent.com/48046183/103544059-d222bf00-4ee2-11eb-9267-5a795f89e272.png" width="40%">

## **Reference**
* **dirent.h** - header file that handles directory files
  * **Structure**
  ```
  struct dirent {
 	long d_ino; 	//l-노드 번호
		off_t	d_off;		//offset
		unsigned short d_reclen;	//파일 이름 길이
		char d_name[NAME_MAX+1];		//파일 이름
  }
  ```
  * **Usage function**
    * **opendir()**
    * **readdir()**
    * **closedir()**
