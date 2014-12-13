#ifndef CMDDEF_H
#define CMDDEF_H

#define LOG_PATH "update.log"
#define TMP_FILE_PATH "filetmp.exe"

#define DOWNLOAD_INFO "download.ini"
#define INSTALL_INFO  "install.ini"

#define SERVER_IP
#define SERVER_PORT
#define USERNAME "quad"
#define PASSWORD

#define SOFTWARE_VERSION    "1.0.0"
#define FIRMWARE_VERSION    "1.0.0"

#define TIMEOUT             10000
#define TIMEOUT_DOWNLOAD    10000
#define MAX_BUFFER_SZIE     (256*1024)

#define START_DOWNLOAD  225
#define FINISH_DOWNLOAD 221
#define TOKEN_CONFIRM   331
#define LOGIN_SUCCESS   230
#define LOGIN_FAILED    530

#define VER_CONFIRM     250
#define VER_NOT_CONFIRM 599

#define NEW_VER         260
#define LASTEST_VER     261

#define GET_ACCEPT      270


#define SOFTWARE        "SOFT"
#define FIRMWARE        "FIRM"

#define CMD_USER        "USER"
#define CMD_VER         "VER (%1,%2)\r\n"
#define CMD_CHECK       "CHECK %1\r\n"
#define CMD_GET         "GET %1\r\n"
#define CMD_READY       "READY\r\n"
#define CMD_CONNECTHOST "CONNECT TO HOST"
#define CMD_READY_REST  "READY %1\r\n"

#endif // CMDDEF_H
