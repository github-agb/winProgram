#pragma once
#ifndef __MODEL
#define __MODEL

#define MS_TYPE_GET 0;
#define MS_TYPE_UPDATE_USERLIST 1;
typedef struct User
{
	int id;
	int gender;
	char name[32];
}USER,*PUSER;

typedef struct Message
{
	int type;
	USER user;
	char message_data[512];
}MESSAGE,*PMESSAGE;

#define DEFAULT_BUFFER_LEN sizeof(MESSAGE)
#endif // !__MODEL

