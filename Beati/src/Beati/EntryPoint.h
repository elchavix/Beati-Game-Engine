#pragma once

#ifdef BE_PLATFORM_WINDOWS 

extern Beati::Application* Beati::CreateApplication();

int main(int argc, char** argv)
{
    // Por ahora...
	Beati::Log::Init();
    BE_CORE_WARN("Beati Engine Initialized!");
    BE_CLIENT_INFO("Hello! penguano");

    auto app = Beati::CreateApplication();
    app->Run();
    delete app;
}

#endif
