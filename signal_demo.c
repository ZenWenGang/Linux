#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// 自定义信号处理函数：必须用异步信号安全的函数（如write）
void sig_handler(int signo) {
    char msg[] = "✅ Caught signal! 信号已被处理\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main() {
    // 1. 初始化sigaction结构体，注册信号处理函数
    struct sigaction act;
    act.sa_handler = sig_handler;       // 指定自定义处理函数
    sigemptyset(&act.sa_mask);           // 信号处理期间不额外阻塞其他信号
    act.sa_flags = 0;                    // 默认标志
    sigaction(SIGUSR1, &act, NULL);      // 注册SIGUSR1信号的处理方式

    // 2. 创建信号集，阻塞SIGUSR1信号
    sigset_t newmask, oldmask, pendmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);        // 把SIGUSR1加入要阻塞的集合
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        perror("sigprocmask 阻塞失败");
        exit(1);
    }
    printf("🔒 已阻塞SIGUSR1信号，现在发送信号会进入未决状态\n");

    // 3. 给当前进程发送SIGUSR1信号（此时信号会被阻塞，进入未决状态）
    printf("📨 正在给当前进程发送SIGUSR1信号...\n");
    raise(SIGUSR1);  // 等价于 kill(getpid(), SIGUSR1);

    // 4. 获取当前未决信号集，检测SIGUSR1是否处于未决状态
    sigpending(&pendmask);
    if (sigismember(&pendmask, SIGUSR1)) {
        printf("⚠️  检测到SIGUSR1处于未决状态（被阻塞，还没处理）\n");
    }

    // 5. 解除SIGUSR1的阻塞，信号会立刻被处理
    printf("\n🔓 解除SIGUSR1阻塞，信号将被处理...\n");
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        perror("sigprocmask 解除阻塞失败");
        exit(1);
    }

    // 6. 再次检测未决信号集，确认信号已被处理
    sigpending(&pendmask);
    if (!sigismember(&pendmask, SIGUSR1)) {
        printf("✅ 再次检测：SIGUSR1已不在未决集合中，处理完成\n");
    }

    // 7. 让程序保持运行，方便你手动发送信号测试
    printf("\n程序将继续运行，你可以在终端用 `kill -USR1 %d` 手动发送信号\n", getpid());
    while (1) {
        sleep(1);
    }

    return 0;
}
