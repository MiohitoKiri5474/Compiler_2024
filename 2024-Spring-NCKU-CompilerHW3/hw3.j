.source hw3.j
.class public Main
.super java/lang/Object

.method public static main([Ljava/lang/String;)V

    .limit stack 128
    .limit locals 128

    ldc "Hello World"
    getstatic java/lang/System/out Ljava/io/PrintStream;
    swap
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
    ldc "\n"
    getstatic java/lang/System/out Ljava/io/PrintStream;
    swap
    invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V
    return
.end method
