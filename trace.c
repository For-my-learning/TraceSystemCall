#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s program [args...]\n", argv[0]);
        return 1;
    }

    pid_t child = fork();
    if (child == 0) {
        // 子プロセス
        ptrace(PTRACE_TRACEME, 0, NULL, NULL); // 自分をトレース対象に
        execvp(argv[1], &argv[1]);            // 指定プログラムを実行
        perror("execvp failed");
        exit(1);
    } else if (child > 0) {
        // 親プロセス
        int status;
        while (1) {
            waitpid(child, &status, 0);       // 子プロセスの停止を待つ
            if (WIFEXITED(status)) break;     // 子が終了したら終了

            // 子プロセスがシステムコールに入ると停止する
            struct user_regs_struct regs;
            ptrace(PTRACE_GETREGS, child, NULL, &regs);

            // x86_64 Linuxの場合、raxレジスタにシステムコール番号が入る
            printf("Syscall: %lld\n", regs.orig_rax);

            // 子を次のシステムコールまで進める
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
    } else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
