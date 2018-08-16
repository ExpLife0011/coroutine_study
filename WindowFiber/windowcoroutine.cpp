#include"windowcoroutine.h"
#include<Windows.h>
// �˳�ջ��С
#define _INT_STACK        (1024 * 1024)
// Ĭ�ϳ�ʼ�������˳���Ŀ
#define _INT_COROUTINE    (16)
#define COROUTINE_DEAD 0
#define COROUTINE_READY 1
#define COROUTINE_RUNNING 2
#define COROUTINE_SUSPEND 3
/*
* �����˳̵�Ԫ coroutine , �����˳̼������� S
*/
// �˳�ջ��С 1M
#define _INT_STACK        (1024 * 1024)
// Ĭ�ϳ�ʼ�������˳���Ŀ
#define _INT_COROUTINE    (16)

/*
* �����˳̵�Ԫ coroutine , �����˳̼������� S
*/
struct coroutine;

struct schedule {
    PVOID main;                 // �˳̹������б������ʱ�˳̶���
    int running;                // ��ǰ�˳̹����������е��˳�id
    int cur_index;              // ��ǰ�˳̼���ѯ�е�ǰ����
    int cap;                    // �˳̼�����,
    int coroutine_count;        // Э�̸���
    struct coroutine ** co;     // ������˳̼�
};

struct coroutine {
    PVOID ctx;                    // ����ϵͳ�˳̶���                
    coroutine_function func;      // �˳�ִ�еĺ�����
    void * ud;                    // �˳�ִ�еĶ������
    int status;       // ��ǰ�˳�����״̬
};
/* ͨ�������߳�ת��Ϊ�˳̣��˴���Ҫ���˳̹رյ�ʱ��ʹ��(���˳�תΪ�߳�)
*
*/
struct schedule* coroutine_open(void)
{
    struct schedule*comanag = (schedule*)malloc(sizeof(struct schedule));
    comanag->cur_index = 0;
    comanag->cap = _INT_COROUTINE;
    comanag->running = -1;
    struct coroutine **co = (coroutine**)calloc(comanag->cap, sizeof(struct coroutine*));
    comanag->co = co;
    comanag->coroutine_count = 0;
	/*ConvertThreadToFiberEx(NULL,??);
	  CreateFiber()��CreateFiberEx()�ķ�ʽ��̫�ʺ��������߳�ת��Э�̣�
      ��Ҫ�ǵ�һ������SIZE_T  dwStackSizeû�취д
	  ��������ڲ���ʹ��ʱ��ֱ����ɷ���Ȩ�޳�ͻ�ˣ�Ӧ����Խ�������
	*/
	comanag->main = ConvertThreadToFiber(NULL);
    return comanag;
}
/*
  ����ĳһ��Э�̵���Ϣ���ݣ����տռ�
*/
inline void _delete_coroutine(coroutine *co) {
    DeleteFiber(co->ctx);
    free(co);
}

inline struct coroutine* _new_coroutine(coroutine_function func, void * ud) {
    struct coroutine *co = (coroutine*)malloc(sizeof(struct coroutine));
    co->func = func;
    co->ud = ud;
    co->status = COROUTINE_READY;
    return co;
}

void coroutine_close(struct schedule* S)
{
    for (int i = 0; i < S->cap; i++) {
        struct coroutine *co = S->co[i];
        if (co) {
            _delete_coroutine(co);
            S->co[i] = NULL;
        }
    }
    free(S->co);
    S->co = NULL;
    free(S);
    S = NULL;
    ConvertFiberToThread();
}

int coroutine_new(struct schedule* S, coroutine_function func, void * ud)
{
    struct coroutine *co = _new_coroutine(func, ud);
    int index = S->cur_index;
    int count = S->cap;
    if (S->cur_index < S->cap) {
        for (int i = 0; i < S->cap; i++) {
            int id = (i + S->coroutine_count) % S->cap;
            if (NULL == S->co[id]) {
                S->co[id] = co;
                ++S->coroutine_count;
                ++S->cur_index;
                return id;
            }
        }
    }
    coroutine ** point = (coroutine **)realloc(S->co, sizeof(struct coroutine *) * S->cap * 2);
    memset(point + S->cap, 0, sizeof(struct coroutine *) * S->cap);
    point[count] = co;
    S->cap *= 2;
    S->co = point;
    S->coroutine_count++;
    return S->cur_index++;
}
/*�ú����������������CreateFiberEx
 * mainfunc(uint32_t low32, uint32_t hi32)
 �˴���Ҫ������Ҳ����ȥ��ѡ�񷽰�CreateFiberEx()
 LPVOID CreateFiberEx(
 SIZE_T                dwStackCommitSize,
 SIZE_T                dwStackReserveSize,
 DWORD                 dwFlags,
 LPFIBER_START_ROUTINE lpStartAddress,
 LPVOID                lpParameter
 );
 LPVOID CreateFiber(
 SIZE_T                dwStackSize,
 LPFIBER_START_ROUTINE lpStartAddress,
 LPVOID                lpParameter
 ); 
 */
static inline VOID WINAPI mainfunc(LPVOID ptr) {
    struct schedule * S = reinterpret_cast<struct schedule*>(ptr);
    int id = S->running;
    struct coroutine * co = S->co[id];

    co->func(S, co->ud);  //  ==>coroutine_function(S,co->ud)
    co = S->co[id];
    co->status = COROUTINE_DEAD;

    SwitchToFiber(S->main);
}

void coroutine_resume(struct schedule* S, int id)
{
    //  ��һЩ���������е�Э�̽��й����϶�����������
    if (S->running != -1) {
        int runningid = S->running;
        coroutine * co = S->co[runningid];
        S->co[runningid] = NULL;
        --S->coroutine_count;
        S->cur_index = runningid;
        S->running = -1;
        _delete_coroutine(co);
        if (runningid == id) {
            return;
        }
    }
    else {
        coroutine * co = S->co[id];
        switch (co->status)
        {
        case COROUTINE_READY:
		
		   co->ctx = CreateFiber(_INT_STACK,  mainfunc,S); 
           // co->ctx = CreateFiberEx(_INT_STACK, 0, ??, mainfunc, S);
        case COROUTINE_SUSPEND:
            co->status = COROUTINE_RUNNING;
            S->running = id;
            S->main = GetCurrentFiber();
            SwitchToFiber(co->ctx);
        default:
            break;
        }
    }
}

void coroutine_yield(struct schedule* S)
{
    if (S) {
        int id = S->running;
        struct coroutine* co = S->co[id];

        co->status = COROUTINE_SUSPEND;
        S->running = -1;
        co->ctx = GetCurrentFiber();

        SwitchToFiber(S->main);
    }
}

int coroutine_status(struct schedule* S, int id)
{
    if (S&&id >= 0 && id<S->cap) {
        if (S->co[id]) {
            return S->co[id]->status;
        }
        return COROUTINE_DEAD;
    }
    return COROUTINE_DEAD;
}

int coroutine_running(struct schedule* S)
{
    if (S) {
        return S->running;
    }
    return -1;
}
