#include "Application.h"
#include "Scene.h"
#include "Global.h"

int main(int argc, char** argv) {
    SUCCESS("Start application");
    auto app = Skynet::CreateApplication();

    /* Layered architecture */
    Skynet::Scene* scene = new Skynet::Scene();
    app->PushLayer(scene);
    app->PushLayer(new Skynet::Global(scene));

    SUCCESS("Application Running");
	app->Run();

    SUCCESS("Finsihed");
	delete app;
}