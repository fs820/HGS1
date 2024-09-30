//------------------------------------
//
//動画再生処理の定義・宣言[movie.h]
//Author
//
//------------------------------------

#include"main.h"

#define MOVIE_OP L"data/MOVIE/OP.mp4"
#define MOVIE_WIN0 L"data/MOVIE/Win0.mp4"
#define MOVIE_WIN1 L"data/MOVIE/Win1.mp4"
#define MOVIE_WIN2 L"data/MOVIE/Win2.mp4"
#define MOVIE_WIN3 L"data/MOVIE/Win3.mp4"
#define MOVIE_LOSE0 L"data/MOVIE/Lose0.mp4"
#define MOVIE_LOSE1 L"data/MOVIE/Lose1.mp4"
#define MOVIE_LOSE2 L"data/MOVIE/Lose2.mp4"
#define MOVIE_LOSE3 L"data/MOVIE/Lose3.mp4"

//プロトタイプ宣言
HRESULT PlayVideo(HWND hwnd, const wchar_t* videoFilePath);