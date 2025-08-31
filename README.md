# TraceSystemCall
for learning linux kernel ,   system programmin, low level programming


fork() 後
親プロセス (child > 0)
┌─────────────┐
│ 親のメモリ  │
└─────────────┘

子プロセス (child == 0)
┌─────────────┐
│ 子のメモリ  │
│ tracer 子コード │
└─────────────┘
        │ execvp(argv[1], &argv[1]) 成功
        ▼
┌─────────────┐
│ 子のメモリ  │
│ 新しいプログラム │  ← 置き換わる
└─────────────┘



