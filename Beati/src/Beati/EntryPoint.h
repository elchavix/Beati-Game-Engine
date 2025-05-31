#pragma once

#ifdef BE_PLATFORM_WINDOWS 

extern Beati::Application* Beati::CreateApplication();

int main(int argc, char** argv)
{
    auto app = Beati::CreateApplication();
    app->Run();
    delete app;
}

#endif
