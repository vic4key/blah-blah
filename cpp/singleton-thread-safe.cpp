#include <iostream>
#include <memory>
#include <mutex>

using namespace std;

class Singleton
{
private:
    inline static mutex m_mutex;
    inline static unique_ptr<Singleton> m_instance = nullptr;

public:
    static Singleton& instance()
    {
        if (m_instance == nullptr)
        {
            lock_guard<mutex> lg(m_mutex);
            m_instance.reset(new Singleton);
        }
        
        return *m_instance;
    }
    
    void method()
    {
        lock_guard<mutex> lg(m_mutex);
        cout << "i am in singleton at " << hex << this << endl;
    }
};

int main()
{
    Singleton::instance().method();
    Singleton::instance().method();
    Singleton::instance().method();

    return 0;
}