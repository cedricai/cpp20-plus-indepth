export module asyncpp.task:loop;

import :queue;
import <cstdint>;
import <chrono>;

namespace asyncpp::task {

export class AsyncTaskLoop {
public:
    // ����������������ѭ���ĵȴ����ʱ�䣨��λΪ���룩
    static const int32_t SLEEP_MS = 1000;

    static AsyncTaskLoop& getInstance();
    static void start() {
        getInstance().startLoop();
    }

private:
    // ֧�ֵ���ģʽ��ͨ��default���η�˵�����캯��ʹ��Ĭ�ϰ汾
    AsyncTaskLoop() = default;
    // ֧�ֵ���ģʽ��ͨ��delete���η�˵���������캯�����ɵ���
    AsyncTaskLoop(const AsyncTaskLoop&) = delete;
    // ֧�ֵ���ģʽ��ͨ��delete���η�˵����ֵ���������ɵ���
    AsyncTaskLoop& operator=(const AsyncTaskLoop&) = delete;

    void startLoop() {
        while (true) {
            loopExecution();
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
        }
    }

    void loopExecution() {
        AsyncTask asyncEvent;
        if (!AsyncTaskQueue::getInstance().dequeue(&asyncEvent)) {
            return;
        }

        asyncEvent.handler();
    }
};

AsyncTaskLoop& AsyncTaskLoop::getInstance() {
    static AsyncTaskLoop eventLoop;

    return eventLoop;
}

}