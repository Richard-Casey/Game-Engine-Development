#pragma once 
class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	enum Status
	{
		/// <summary>
		/// The menu
		/// </summary>
		Menu,
		/// <summary>
		/// The game
		/// </summary>
		Game,
		/// <summary>
		/// The end
		/// </summary>
		End
	};

	void Update();

};

