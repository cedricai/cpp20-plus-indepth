export module asyncpp.task:coroutine;

import <coroutine>;
import <functional>;

namespace asyncpp::task {
    // Э����
    export struct Coroutine
    {
        // Э��Promise����
        struct promise_type
        {
            //std::function<void()> _doneHook;
            //bool _doneHookExecuted = false;

            Coroutine get_return_object() { 
                return {
                    ._handle = std::coroutine_handle<promise_type>::from_promise(*this) 
                }; 
            }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() {}
        };

        // Э�̵ľ���������ڹ���Coroutine�࣬����ҵ������е��ýӿڽ�����ز���
        std::coroutine_handle<promise_type> _handle;

        //bool done() const {
        //    return _handle.done();
        //}

        //void destroy() {
        //    _handle.destroy();
        //}

        //bool next() {
        //    if (!_handle.done()) {
        //        _handle.resume();

        //        return true;
        //    }

        //    auto& promise = _handle.promise();
        //    if (promise._doneHookExecuted) {
        //        return false;
        //    }

        //    if (promise._doneHook) {
        //        promise._doneHook();
        //    }

        //    promise._doneHookExecuted = true;

        //    return false;
        //}

        //bool hasDoneHook() const{
        //    return static_cast<bool>(_handle.promise()._doneHook);
        //}

        //void registerDoneHook(std::function<void()> doneHook) {
        //    _handle.promise()._doneHook = doneHook;
        //}
    };

    // AsyncTaskSuspender��������
    export template <typename ResultType>
    struct Awaitable;
    export using AsyncTaskResumer = std::function<void()>;
    export using CoroutineHandle = std::coroutine_handle<Coroutine::promise_type>;
    export template <typename ResultType>
    using AsyncTaskSuspender = std::function<void(
        Awaitable<ResultType>*, AsyncTaskResumer, CoroutineHandle&
    )>;

    // Awaitable���Ͷ��壨���������������Ͳ�Ϊvoidʱ��
    export template <typename ResultType>
    struct Awaitable
    {
        // co_awaitʱ��Ҫִ�е����񣬿����߿�����suspendʵ���е��øú���ִ���û�����������
        std::function<ResultType()> _taskHandler;
        // �洢����ִ�еĽ��������await_resume����Ϊco_await���ʽ��ֵ���ء�
        ResultType _taskResult;
        // �洢�������Զ����await_suspendʵ�֣�����await_suspend�е���
        AsyncTaskSuspender<ResultType> _suspender;

        bool await_ready() { return false; }
        void await_suspend(CoroutineHandle h)
        {
            _suspender(this, [h] { h.resume(); }, h);
        }

        ResultType await_resume() {
            return _taskResult;
        }
    };

    // Awaitable���Ͷ��壨����������������Ϊvoidʱ��
    export template <>
    struct Awaitable<void>
    {
        // co_awaitʱ��Ҫִ�е����񣬿����߿�����suspendʵ���е��øú���ִ���û�����������
        std::function<void()> _taskHandler;
        // �洢�������Զ����await_suspendʵ�֣�����await_suspend�е���
        AsyncTaskSuspender<void> _suspender;

        bool await_ready() { return false; }
        void await_suspend(CoroutineHandle h)
        {
            _suspender(this, [h] { h.resume(); }, h);
        }

        void await_resume() {}
    };
}
