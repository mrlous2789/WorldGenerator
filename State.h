#pragma once
namespace Mer
{
	class State //base state class 
	{
	public:
		virtual void Init() = 0;
		virtual void HandleInput() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void CleanUp() = 0;

		virtual void Pause() { }
		virtual void Resume() { }
	private:

	};
}