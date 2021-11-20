### ❕과제 목표

‼️client와 server라는 실행 프로그램을 만든다.

각각 실행하는 방법은:

./server

pid를 알려준다. getpid()함수를 사용할 수 있다.

pause()상태에 걸린다.

./client <server pid> <send message>

server에서 보여준 pid와 보낼 메세지를 적어 실행시키면 

server을 실행시켜둔 창에서 메세지\n가 출력된다.

‼️아래에 나올 이외 여러 조건들이 있다. 

### -1. server에서 pid출력하기 & pause()상태 만들기

‼️조건: getpid()라는 함수가 허용되어있다.

```c
ft_putnbr(getpid());
ft_putchar("\n");
while(1)
	pause();
```

### -1. client에 정상 입력 확인 & 입력된 문자 길이 저장

```c
if(argc < 3)
	return (-1);

len = ft_strlen(argv[2]) + 1;
```

### 0. sigaction함수

‼️조건: sigaction함수가 허용되어있다.

<aside>
💡 int    sigaction(int signum, const struct sigaction *act, sturct sigaction *oldact)

</aside>

인자는 순서대로 들어온 시그널, 처리할 방법의 구조체 주소, 기존 처리방법 구조체 주소

ex> sigaction(SIGUSR1, action, 0): SIGUSR1이라는 시그널을 action구조체에 적힌대로 처리하고 이전 처리방법은 저장하지 않는다

### 1. struct sigaction 구조체 (action)

- `#include <signal.h>`에 정의되어 있다.

```c
struct sigaction
{
	void (*sa_handler)(int);
		// signal을 처리하기 위한 핸들러
	void (*sa_sigaction)(int, siginfo_t*, void*);
		// signal을 처리하기 위한 핸들러2
		// 인자는 순서대로 처리할 시그널, 처리할 방법의 구조체 주소, 기존 처리방법 구조체 주소
	sigset_t sa_mask;
		//시그널을 처리하는 동안 블록(나중에 처리하게 예약)시킬 시그널을 모아놓은 변수이다
		//따라서 sigemptyset()을 통해 action구조체의 sa_mask변수를 비워준다면 모든 시그널이 블로킹되지 않는다
	int sa_flags;
		//옵션설정: ex>SA_SIGINFO : sa_handler 대신 sa_sigaction을 작동시킴
		//더 자세한 flag는 -> https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=skssim&logNo=121271980
	void (*sa_restorer)(void);  
		// 이번 과제에서는 사용X
}
```

함수 내에 `struct sigaction    action`; 으로 sigaction구조체에 action이라는 이름을 달아 사용한다.

~~구조체 공부는 여기서:(노트필기 옮기겟습니다...)~~

<aside>
⚠️ sigaction함수와 sigaction구조체의 sa_sigaction함수는 다르다!
비슷해 보이지만 처리할 방법이 담긴 구조체 자체가 다름

</aside>

우선 대충 sigaction구조체도 함수도 시그널을 받고 어떻게 처리해 준다는건 알겠다.

### 2. signal이란?

‼️조건: signal함수가 허용되어있다.

<aside>
💡 void    (*signal(int signum, void (*handler)(int)))(int);

</aside>

*참고로 위에서 말하던 signal은 이 함수에서의 signum에 들어간다.*

```c
void interruptHandler(int sig){
        printf("this program will be exited in 3 seconds..\n");
        sleep(3);
        exit(0);
}
int main(){
        signal(SIGINT, interruptHandler); 
        printf("input Ctrl+C\n");
        while(1);
}

실행후 Ctrl+C를 눌러 SIGINT신호를 보내주면 interrupHandler함수가 실행된다.
```

2번 시그널인 SIGINT는 `#define SIGINT 2`로 매크로화 되어있으며 Ctrl+C로 신호를 보낼 수 있다.

위 코드 출처 & SIGINT 이외의 더 많은 종류는 → [https://reakwon.tistory.com/46](https://reakwon.tistory.com/46)

‼️조건: 이번 과제에서 사용할 수 있는 시그널은 SIGUSR1, SIGUSR2 두가지다.

즉 우리는 SIGUSR1이라는 시그널이 들어왔을 때 작동시킬 handler 하나와

SIGUSR2라는 시그널이 들어왔을 때 작동시킬 handler 하나

이렇게 2개만 가지고 client프로그램으로부터 server프로그램으로 메세지를 전달받게 해야한다.

보낼수 있는 시그널이 2개라는 점에서 이미 2진법과 비트연산을 사용해야 한다는 것을 떠올렸다.

그렇다면 어떻게 비트연산으로 메세지를 보낼것인가?

### 3. 비트마스킹

<aside>
💡 *1 & 1 → 1*
&연산은 두 입력이 모두 1일때만 1을 출력하고 나머진 전부 0을 출력한다.

</aside>

보내려는 메세지 `"a" == 97 == (0b) 0110 0001` 과

기본값 `(0b) 1000 0000` 에서 하나씩 >>하며

&연산하면...

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/8e379e42-7a0e-421b-853a-5e2e7ddc60fd/Untitled.png)

따라서 1000 0000 부터 1의 위치를 >>(오른쪽으로 이동) 하며 어떤 문자가 들어왔는지 스캔하는 것이다.

초기에  문자 길이를  len에 저장해두었으니 len만큼 반복한다.

 `_signal`구조체의 `bit`(unsigned int임)에 기초가 되는 0b 1000 0000를 저장해놓고 전부 0이 될 때마다 초기화해주도록 하자

```c
_signal.bit = 0b1000000;
```

이제 우리는 _signal.bit를 >>, & 하면 들어온 문자를 2진법으로 쪼개 스캔하여 1인지 0인지 알아낼 수 있다.

하지만 아직 우리는 client안에 있다.

1인지 0인지 알아냈다면 그걸  SIGUSR1, SIGUSR2로 어떻게 바꿀것인가...?

### 4. kill함수

‼️조건: kill함수가 허용되어있다.

[https://man7.org/linux/man-pages/man2/kill.2.html](https://man7.org/linux/man-pages/man2/kill.2.html)

<aside>
💡 int    kill(pid_t pid, int sig);

</aside>

kill이라해서 실행을 종료시키나...를 생각했지만 그건 sig으로 SIGKILL을 주었을때이다.

즉 pid를 인자로 받아 그 곳에 sig를 보내는 함수!

(참고로 반환값은 성공시 0 실패시 -1을 반환한다.)

```c
_signal.pid = ft_atoi(argv[1]);

while (_signal.bit)
{
	if(argv[2] & _signal.bit)
		kill(_signal.pid, SIGUSR1);
	else
		kill(_signal.pid, SIGUSR2);
}
```

*client를 실행시킬 때 `server의 pid`(==argv[1])와 `보낼 메세지`(==argv[2])를 받았음*

이제 server로 SIGUSR신호를 보내는 것도 문제 없다

### 5. server에서 문자 수신하기

이제 생각해보자...

현재 우리는 SIGUSR1 혹은 2가 줄지어 들어오기 직전의 상황이다.

SIGUSR1은 1, SIGUSR2는 0이다.

시그널을 받아 핸들링 하려면

1. 둘을 각자 다른 핸들링 구조체or함수에 넣어주거나
2. 한군데 넣어서 if문으로 구분하는 방법이 있다.

2번을 선택했다.

 `sigaction(SIGUSR1, &action, 0)`, `sigaction(SIGUSR2, &action, 0)`

어느 신호든 받아 action으로 들어와 handle해줄 함수를 찾을것이다

1. sa_handler로 해주기
2. sa_sigaction로 해주기

siginfo_t구조체의 값을 사용해야하기 때문에 2번으로 진행한다.

sa_sigaction으로 들어와 어떻게 2진법으로 쪼개진 문자를 다시 기존 문자로 바꿔준다.

우선 기존 문자를 담을 변수를 하나 지정한다.

`_signal`구조체의 `word`라고 지어주겠다 (당연히 char형임)

이곳에 SIGUSR1일때만 1을 복사(해당 자리에 1을 넣어야하니 `+=`)해주고 >> 하여 다음으로 넘긴다.

SIGUSR2일때는 그냥 >>해준다! 어짜피 0에다 0을 더해도 안더해도 0이니

```c
if (sig == SIGUSR1)
		_signal.word += _signal.bit;
_signal.bit >>= 1;
```

이제 word에 client가 보낸 메세지가 8비트어치(한글자) 담겼을것이다.

bit에서 전부 가져왔는지 확인 후 (bit == 0) 전부 복사했다면 word를 putchar로 출력해준다.

앗! 문자열이 안들어왔다면? 일단 출력했는데 그게 마지막 글자 였다면? (word == 0) gnl에서 그랬듯 "\n"을 출력해준다.

한 글자 스캔과 출력이 끝났으니 bit와 word를 다시 초기화해준다.

usleep()을 통해 글자 출력 사이에 시간을 준다. 

*(시간을 주는 이유는 못 찾음 출력 빠르면 좋은거 아닌가?)*

아까 위에서 siginfo_t구조체의 값을 사용하기 위해 sa_sigaction을 선택했다고 하였다.

### 6. siginfo_t구조체?

~~이거 왜 안끝나지~~

- [우선은 이렇게 있는데...](https://man7.org/linux/man-pages/man2/sigaction.2.html)
    
    ```c
    int si_signo; //시그널 넘버
    int si_errno; //errno 값
    int si_code; //시그널 코드
    pid_t si_pid; //프로세스 ID 보내기
    uid_t si_uid; //프로세스를 전송하는실제 사용자 ID
    int si_status; //Exit값 또는 시그널
    clock_t si_utime; //소모된 사용자 시간
    clock_t si_stime; //소모된 시스템 시간
    sigval_t si_value; //시근러 값
    int si_int; //POSIX.1b 시그널
    void* si_ptr; //POSIX.1b 시그널
    void* si_addr; //실패를 초래한 메모리 위치
    int si_band; //밴드 이벤트
    int si_fd; //파일 기술자
    ```
    

우리가 볼 건 si_pid뿐이다.

- [그리고 문서는 이쪽이 더 정확한것같다](https://www.qnx.com/developers/docs/6.5.0SP1.update/com.qnx.doc.neutrino_lib_ref/s/siginfo_t.html)🤔
    
    ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/c5d8c66e-1387-4878-bcc0-ac862988185a/Untitled.png)
    

siginfo_t구조체의 si_pid는 client의 pid를 가져온다.

kill함수를 통해 client에 끝났다는 시그널을 보낸다 (1)

kill(act→si_pid, SIGUSR1)...

### 7. client에 SIGUSR1이 들어왔다!

```c
sigaction(SIGUSR1, &action, 0)
```

siginfo_t구조체를 사용할 일이 없으니 

sa_handler를 사용해 _signal.word >>= 1 한다.

### 8. 유니코드

࿚ ࿎ ࿏ 똟 ࿿



[https://unicode-table.com/kr/#basic-latin](https://unicode-table.com/kr/#basic-latin)

emoji:
🖤∑∞☞★⚝✅🔥⌚☣☮🌏📱Ⅻ🚀€🍔🍦👑⚽🎵🎧♻

200자 (400byte):
★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

strlen으로 한 글자마다 전송이 끝났다는 신호를 보내기 때문에 4바이트를 차지하는 유니코드도 잘 전송됨 [https://stackoverflow.com/questions/10017328/unicode-stored-in-c-char](https://stackoverflow.com/questions/10017328/unicode-stored-in-c-char)

### 9. unused parameter 처리

[https://stackoverflow.com/questions/3599160/how-to-suppress-unused-parameter-warnings-in-c/12891181](https://stackoverflow.com/questions/3599160/how-to-suppress-unused-parameter-warnings-in-c/12891181)

```c
action.sa_sigaction = &server_sa_sigaction;

void server_sa_sigation(int i)
{
		(void)i;
		_signal.word_bit >>= 1;
}
```
