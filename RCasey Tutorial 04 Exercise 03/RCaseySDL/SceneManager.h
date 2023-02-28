#pragma once 
class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	enum Status
	{
		Menu,
		Game,
		End
	};

	void Update();

};

