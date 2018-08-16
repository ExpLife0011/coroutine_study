/* Copyright 2018
 �ر�˵�����˴������ϸ�ο��Ʒ�Ŀ�coroutine(linux C++����),����Fiber
 github ��ַ��https://github.com/cloudwu/coroutine
 ��֪���᲻�����ڴ���������,���������һ���˳̵��ô�
 ��������:
    https://docs.microsoft.com/en-us/windows/desktop/api/WinBase/nf-winbase-switchtofiber 
    windows�����и�co_yield�������Ǹ��ؼ���
 �����߳�תΪ�˳̣����ֱ�Ӵ���Э�̣�CreateFiber������Ȩ�޳�ͻ��ֱ�ӱ�
    https://docs.microsoft.com/zh-cn/windows/desktop/api/winbase/nf-winbase-convertthreadtofiber
 ���˳�תΪ���̣߳���������������ָ�벻֪������ȥ��(����ʱ��������ˣ�������,Э�̵ĵ�ַû���ֹ���������Э��Ҳ����һ��Э��)��
    https://docs.microsoft.com/zh-cn/windows/desktop/api/winbase/nf-winbase-convertfibertothread
 
 */

#ifndef WINDOWCOROUTINE_H_
#define WINDOWCOROUTINE_H_

/*Э�̹����*/
struct schedule;

/*
* ���������˳̵�����, ��ͬ���˳̴�����Ҫִ�еĺ�����.
* ���������� �������صĽ��
* �����û��Զ�������
*/
typedef void(*coroutine_function)(void* schedule, void * ud);

/*
* �����˳�ϵͳ, ���������˳�
* ���ؿ������˳̵���ϵͳ������
*/
extern struct schedule* coroutine_open(void);

/*
* �رտ������˳�ϵͳ
* S ��co_start ���ص��˳̹�����
*/
extern void coroutine_close(struct schedule*S);
/*
* ����һ���˳̶���,�����ش����˳̵�id. �����õ��˳�״̬��CS_Ready
* S �� coroutine_start ���ص��˳̹�����
* func ���˳����е�����
* ud  �� �û����������, coroutine_function ����Ĳ���(args)
*  ���ش����õ��˳̱�ʶid(idָ���ǵ�ǰ�Ĺ������е����λ��)
*/
extern int coroutine_new(struct schedule* S, coroutine_function func, void * ud);

/*
* resume�ָ��˳̶���.
* S   ��Ϊ�˳̹���������
* id  ָ����coroutine_create �������˳̶���
* ������׼��״̬�͹���״̬���˳̶��󼤻
*/
extern void coroutine_resume(struct schedule* S, int id);

/*
* yield ����ָ���жϵ�ǰ���еĵ��˳�, ����CPU�������˳̴������.
* S �˳̹���������  
*/
extern void coroutine_yield(struct schedule*S);

/*
* ��ȡ��ǰ�˳����е�״̬
* S  �˳̹���������
* id ���� coroutine_create �������˳̶��󣬾����˳��ڹ������е�λ��
*����״̬
*/
extern int  coroutine_status(struct schedule*S, int id);

/*
* �õ���ǰ�˳�ϵͳ�����е��˳�
* S�˳̹���������
* ���ص�ǰ���е��˳�id,-1��ʾû���˳�������
*/
extern int coroutine_running(struct schedule*S);

#endif // WINDOWCOROUTINE_H_

