#include <Windows.h>
#include <ShellAPI.h>
#include <string>
#include <tchar.h>


int main() {
    
    // HINSTANCE result = ShellExecute(
    //     NULL,           // 父窗口句柄（NULL 表示无父窗口）
    //     _T("runas"),     // 操作类型（"open" 表示执行程序）
    //     _T("powershell"), // 目标程序
    //     _T("-ExecutionPolicy Bypass -Command \"Set-ExecutionPolicy RemoteSigned -Scope CurrentUser\""),        // 命令行参数
    //     NULL,            // NULL为默认工作目录
    //     SW_SHOW         // 显示窗口
    // );

    return 0;
}

