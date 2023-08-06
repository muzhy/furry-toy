#ifndef __FURRY_TOY_ERROR_H__
#define __FURRY_TOY_ERROR_H__

namespace furry_toy
{   
    /*********************************************
    * 全局通用的错误码定义
    * 错误码值为32位整型，前8位表示产生此错误的模块
    * 后24位由模块内部自定义
    * 0x00 系统保留错误码，用于表示全局错误
    * 0x01 网络错误
    **********************************************/
    enum ErrorCodeEnum
    {
        SUCCESS                 = 0x00000000,
        UNKNOW                  = 0x00000001,

        NET_SOCKET_INIT_ERR     = 0x01000001,
        NET_SOCKET_BIND_ERR     = 0x01000002,
        NET_SOCKET_LISTEN_ERR   = 0x01000003,
    };

    // const char* getMsgFromCode(const ErrorCodeEnum& code)
    // {
    //     switch(code)
    //     {
    //         case SUCCESS:
    //             return "Success";
    //         case UNKNOW:
    //             return "Unknow err!";
    //         case NET_SOCKET_INIT_ERR:
    //             return "Init socket failed!";
    //         case NET_SOCKET_BIND_ERR:
    //             return "Socket bind to port failed!";
    //         case NET_SOCKET_LISTEN_ERR:
    //             return "Socket listen failed!";
    //         default:
    //             return "Undefine error code!";
    //     }
    // }
}

#endif