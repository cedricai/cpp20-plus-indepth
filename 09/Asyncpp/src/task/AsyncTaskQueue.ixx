export module asyncpp.task:queue;

import <functional>;
import <mutex>;
import <vector>;

namespace asyncpp::task {

export struct AsyncTask {
    // �첽������������
    using Handler = std::function<void()>;

    // �첽��������
    Handler handler;
};

export class AsyncTaskQueue {
public:
    static AsyncTaskQueue& getInstance();

    void enqueue(const AsyncTask& item) {
        std::lock_guard<std::mutex> guard(_queueMutex);

        _queue.push_back(item);
    }

    bool dequeue(AsyncTask* item) {
        std::lock_guard<std::mutex> guard(_queueMutex);

        if (_queue.size() == 0) {
            return false;
        }

        *item = _queue.back();
        _queue.pop_back();

        return true;
    }

    size_t getSize() const {
        return _queue.size();
    }

private:
    // ֧�ֵ���ģʽ��ͨ��default���η�˵�����캯��ʹ��Ĭ�ϰ汾
    AsyncTaskQueue() = default;
    // ֧�ֵ���ģʽ��ͨ��delete���η�˵���������캯�����ɵ���
    AsyncTaskQueue(const AsyncTaskQueue&) = delete;
    // ֧�ֵ���ģʽ��ͨ��delete���η�˵����ֵ���������ɵ���
    AsyncTaskQueue& operator=(const AsyncTaskQueue&) = delete;

    // �첽�������
    std::vector<AsyncTask> _queue;
    // �첽������л�����������ʵ���߳�ͬ����ȷ�����в������̰߳�ȫ
    std::mutex _queueMutex;
};

AsyncTaskQueue& AsyncTaskQueue::getInstance() {
    static AsyncTaskQueue queue;

    return queue;
}

}