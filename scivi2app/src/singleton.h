#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
public:
    static T& instance() {
        static T instance;
        return instance;
    }
protected:
    Singleton() {}
public:
    Singleton(Singleton const&) = delete;
    void operator=(Singleton const&) = delete;
};

#endif // SINGLETON_H
