#pragma once

#ifdef BE_PLATFORM_WINDOWS 

extern Beati::Application* Beati::CreateApplication();

int main(int argc, char** argv)
{
    // Por ahora...
	Beati::Log::Init();
    BE_CORE_WARN("Beati Engine Initialized!");

	BE_PROFILE_BEGIN_SESSION("Startup", "BeatiProfile-Startup.json");
    auto app = Beati::CreateApplication();
	BE_PROFILE_END_SESSION();

    BE_PROFILE_BEGIN_SESSION("Runtime", "BeatiProfile-Runtime.json");
    app->Run();
	BE_PROFILE_END_SESSION();

	BE_PROFILE_BEGIN_SESSION("Shutdown", "BeatiProfile-Shutdown.json");
    delete app;
	BE_PROFILE_END_SESSION();
}

#endif
