#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(const std::string& eveniment) = 0;
};

#endif // OBSERVER_H