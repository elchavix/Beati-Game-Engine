#include <Beati.h>

class Sandbox : public Beati::Application
{
public:
    Sandbox() {}
    ~Sandbox() {}
};

Beati::Application* Beati::CreateApplication()
{
    return new Sandbox();
}