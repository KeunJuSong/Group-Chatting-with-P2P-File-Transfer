# **Group Chatting**

## **Overview**
* 1 server, 2 more clients architecture based on Server-Client Model
* In this part, several fork() calls are required
* Using ```pipe()``` that can communicate between parent process and child process
* If you want more details about ```pipe()```, refer **[Report](https://github.com/KeunJuSong/Group-Chatting-with-P2P-File-Transfer/blob/master/Embedded_OS_5T_TermProject_Report(%232%2C%233).docx)** (It is written in Korean)

## **Description**
* Design as model structure below
<img src="https://user-images.githubusercontent.com/48046183/103538213-26c13c80-4ed9-11eb-8fea-96d0d2995e7b.png" width="90%">

* **Architecture** - Asysmmetric Group Chatting System with ```pipe()```   
<img src="https://user-images.githubusercontent.com/48046183/103539547-a3edb100-4edb-11eb-98df-eba9c1072010.png" width="70%">

## **Results**
* **Server**
<img src="https://user-images.githubusercontent.com/48046183/103539857-1eb6cc00-4edc-11eb-97c1-55a9fc923250.png" width="50%">

* **Client 1**
<img src="https://user-images.githubusercontent.com/48046183/103539991-59206900-4edc-11eb-8e9c-522975c5de97.png" width="50%">

* **Client 2**
<img src="https://user-images.githubusercontent.com/48046183/103540106-8836da80-4edc-11eb-9791-75b64094ec79.png" width="50%">

## **Reference**
* **pipe()** 
  * https://www.geeksforgeeks.org/pipe-system-call/
