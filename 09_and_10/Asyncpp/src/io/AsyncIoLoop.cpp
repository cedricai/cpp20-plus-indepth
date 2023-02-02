export module asyncpp.io:loop;

import :task;
import asyncpp.task;

import <thread>;
import <chrono>;
import <functional>;

namespace asyncpp::io {
    export class AsyncIoLoop {
    public:
        // ����������������ѭ���ĵȴ����ʱ�䣨��λΪ���룩
        static const int32_t SLEEP_MS = 1000;

        static AsyncIoLoop& start();

    private:
        // ֧�ֵ���ģʽ�����䶨��δprivate����ֹ�ⲿ���ù��캯��
        AsyncIoLoop() {
            _thread = std::jthread(std::bind(&AsyncIoLoop::loopMain, this));
        }
        // ֧�ֵ���ģʽ��ͨ��delete���η�˵���������캯�����ɵ���
        AsyncIoLoop(const AsyncIoLoop&) = delete;
        // ֧�ֵ���ģʽ��ͨ��delete���η�˵����ֵ���������ɵ���
        AsyncIoLoop& operator=(const AsyncIoLoop&) = delete;

        void loopExecution() {
            AsyncIoTask opItem;
            if (!AsyncIoTaskQueue::getInstance().dequeue(&opItem)) {
                return;
            }

            opItem.taskHandler();

            auto& asyncEventQueue = asyncpp::task::AsyncTaskQueue::getInstance();
            asyncEventQueue.enqueue({
                .handler = opItem.resumeHandler
            });
        }

        void loopMain() {
            while (true) {
                loopExecution();
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            }
        }
        
        // jthread����ΪI/O�̣߳�jthread�ø��߳̽���֮ǰ�������̶��������
        std::jthread _thread;
    };

    AsyncIoLoop& AsyncIoLoop::start() {
        static AsyncIoLoop ioLoop;

        return ioLoop;
    }
}
