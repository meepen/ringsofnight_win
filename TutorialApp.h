#ifndef TUTORIALAPP_H
#define TUTORIALAPP_H

#include "MethodDispatcher.h"
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include "sector.h"
#include "player.h"
#include "game.h"
#include "commandwriter.h"
#include "MainWindow.h"
#include "ChatMessage.h"
#include "application.h"

extern GAME game;

using namespace Awesomium;

class TutorialApp : public Application::Listener {
public:
	Application *app_;
	View *view_;
	MethodDispatcher dispatcher;

	TutorialApp()
		: app_(Application::Create()),
		view_(0) {
		app_->set_listener(this);
	}

	virtual ~TutorialApp() {
		if (view_)
			app_->DestroyView(view_);
		if (app_)
			delete app_;
	}

	void Run() {
		app_->Run();
	}

	// Inherited from Application::Listener
	virtual void OnLoaded() {
		view_ = View::Create(640, 400);

		BindMethods(view_->web_view());

		TCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);
		std::string url = "file:///";
		url.append(path);
		url.append("/game.html");
		printf("%s\n", url.c_str());
		view_->web_view()->LoadURL(WebURL(WSLit(url.c_str())));
	}

	// Inherited from Application::Listener
	virtual void OnUpdate() {
	}

	// Inherited from Application::Listener
	virtual void OnShutdown() {
	}

	void BindMethods(WebView *v)
	{

		JSValue &jsgame = v->CreateGlobalJavascriptObject(WSLit("game"));
		if (jsgame.IsObject())
		{
			JSObject &obj = jsgame.ToObject();
			dispatcher.Bind(obj, WSLit("sendChat"), JSDelegate(this, &TutorialApp::SendChat));
			dispatcher.BindWithRetval(obj, WSLit("getLocalResources"), JSDelegateWithRetval(this, &TutorialApp::GetLocalResources));
			dispatcher.BindWithRetval(obj, WSLit("getX"), JSDelegateWithRetval(this, &TutorialApp::GetX));
			dispatcher.BindWithRetval(obj, WSLit("getY"), JSDelegateWithRetval(this, &TutorialApp::GetY));
			dispatcher.BindWithRetval(obj, WSLit("getU"), JSDelegateWithRetval(this, &TutorialApp::GetU));
			JSArray &sectors = JSArray(10);
			for (int u = 0; u < 10; u++)
			{
				JSArray &universe = JSArray(40);
				sectors.Insert(universe, u);
				for (int x = 0; x < 40; x++)
				{
					JSArray xs = JSArray(40);
					universe.Insert(xs, x);
					for (int y = 0; y < 40; y++)
					{
						JSObject sector = JSObject();
						sector.SetProperty(WSLit("init"), JSValue(false));
						xs.Insert(sector, y);
						printf("%i %i %i\n", u, x, y);
					}
				}
			}
			obj.SetProperty(WSLit("sectors"), sectors);
		}
		v->set_js_method_handler(&dispatcher);
	}

	JSValue GetX(WebView *caller, const JSArray &args)
	{
		RON::Player *ply = game.GetPlayer(game.local);
		if (ply)
			return JSValue(ply->x);
		return JSValue::Undefined();
	}
	JSValue GetU(WebView *caller, const JSArray &args)
	{
		RON::Player *ply = game.GetPlayer(game.local);
		if (ply)
			return JSValue(ply->universe);
		return JSValue::Undefined();
	}
	JSValue GetY(WebView *caller, const JSArray &args)
	{
		RON::Player *ply = game.GetPlayer(game.local);
		if (ply)
			return JSValue(ply->y);
		return JSValue::Undefined();
	}

	JSValue GetLocalResources(WebView *caller, const JSArray &args)
	{
		JSArray ret(5);
		RON::Player *ply = game.GetPlayer(game.local);
		if (ply)
		{
			for (int i = 0; i < 5; i++)
			{
				ret.Insert(JSValue(ply->resources[i]), i);
			}
			return ret;
		}
		return Awesomium::JSValue::Undefined();
	}

	// Bound to game.sendChat()
	void SendChat(WebView *caller, const JSArray &args)
	{
		JSValue val = args.At(0);
		if (val.IsInteger())
		{
			char chan = (char)val.ToInteger();
			std::string text("");
			for (unsigned int i = 1; i < args.size(); i++)
			{
				WebString str = args.At(i).ToString();
				char *tmp = new char[str.length() + 1];
				tmp[str.length()] = 0;
				str.ToUTF8(tmp, str.length());
				text += tmp;
				delete[]tmp;
			}
			game.writer->write(new RON::ChatMessage(text.c_str(), chan));
		}
	}
};

#endif