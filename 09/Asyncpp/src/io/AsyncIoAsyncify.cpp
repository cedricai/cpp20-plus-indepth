export module asyncpp.io:asyncify;

import <coroutine>;
import <type_traits>;
import asyncpp.core;
import asyncpp.task;
import :task;

namespace asyncpp::io {
    using asyncpp::core::Invocable;
    using asyncpp::task::Awaitable;
    using asyncpp::task::AsyncTaskResumer;
    using asyncpp::task::asyncify;
    using asyncpp::task::AsyncTaskSuspender;
    using asyncpp::task::CoroutineHandle;

    // �첽I/O�����AsyncTaskSuspender�����������������Ͳ�Ϊvoidʱ��
    template <typename ResultType>
    void ioAsyncAwaitableSuspend(
        Awaitable<ResultType>* awaitable,
        AsyncTaskResumer resumer,
        CoroutineHandle& h
    ) {
        asyncpp::io::AsyncIoTask operationItem{
            .resumeHandler = [h] {
                h.resume();
            },
            .taskHandler = [awaitable]() {
                awaitable->_taskResult = awaitable->_taskHandler();
            }
        };

        asyncpp::io::AsyncIoTaskQueue::getInstance().enqueue(operationItem);
    }

    // �첽I/O�����AsyncTaskSuspender������������������Ϊvoidʱ��
    template <>
    void ioAsyncAwaitableSuspend<void>(
        Awaitable<void>* awaitable,
        AsyncTaskResumer resumer,
        CoroutineHandle& h
    ) {
        asyncpp::io::AsyncIoTask operationItem{
            .resumeHandler = [h] {
                h.resume();
            },
            .taskHandler = [awaitable]() {
                awaitable->_taskHandler();
            }
        };

        asyncpp::io::AsyncIoTaskQueue::getInstance().enqueue(operationItem);
    }

    // �첽�����ߺ�����֧�ֽ���ͨ����f��ΪI/O�����첽��
    export template <Invocable T>
    auto ioAsyncify(T taskHandler) {
        using ResultType = std::invoke_result_t<T>;

        AsyncTaskSuspender<ResultType> suspender = ioAsyncAwaitableSuspend<ResultType>;
        return asyncify(taskHandler, suspender);
    }
}