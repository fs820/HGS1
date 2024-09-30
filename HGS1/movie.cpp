//------------------------------------
//
//動画再生処理[movie.cpp]
//Author:NULL
//
//------------------------------------

#include <mfapi.h>
#include <mfidl.h>
#include <mfplay.h>
#include <mfobjects.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <evr.h>
#include <assert.h>
#include <Windows.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "evr.lib")

// 音声と映像のメディアタイプを定義
const GUID MFMediaType_Audio = { 0x73647561, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
const GUID MFMediaType_Video = { 0x73646976, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

// エラーチェック用マクロ
#define CHECK_HR(hr) if (FAILED(hr)) { return hr; }

//---------------------------------------------------------
// トポロジの作成ヘルパー関数
//---------------------------------------------------------
HRESULT AddStreamToTopology(IMFTopology* pTopology, IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, DWORD iStream, HWND hwnd)
{
    IMFStreamDescriptor* pSD = NULL;
    IMFActivate* pRendererActivate = NULL;
    BOOL fSelected = FALSE;
    HRESULT hr = S_OK;

    // ストリームディスクリプタを取得
    hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD);
    CHECK_HR(hr);

    if (fSelected)
    {
        // ストリームタイプを判別
        GUID majorType;
        IMFMediaTypeHandler* pHandler = NULL;
        hr = pSD->GetMediaTypeHandler(&pHandler);
        CHECK_HR(hr);

        hr = pHandler->GetMajorType(&majorType);
        pHandler->Release();
        CHECK_HR(hr);

        // 映像ストリームならEVRを使い、音声ストリームならSARを使う
        if (majorType == MFMediaType_Video)
        {
            // EVRのアクティベータを作成
            hr = MFCreateVideoRendererActivate(hwnd, &pRendererActivate);
            CHECK_HR(hr);
        }
        else if (majorType == MFMediaType_Audio)
        {
            // SARのアクティベータを作成
            hr = MFCreateAudioRendererActivate(&pRendererActivate);
            CHECK_HR(hr);
        }

        // トポロジにソースノードを追加
        IMFTopologyNode* pSourceNode = NULL;
        hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pSourceNode);
        CHECK_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
        CHECK_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
        CHECK_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
        CHECK_HR(hr);

        hr = pTopology->AddNode(pSourceNode);
        CHECK_HR(hr);

        // トポロジにレンダラーノードを追加
        IMFTopologyNode* pOutputNode = NULL;
        hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pOutputNode);
        CHECK_HR(hr);

        hr = pOutputNode->SetObject(pRendererActivate);
        CHECK_HR(hr);

        hr = pTopology->AddNode(pOutputNode);
        CHECK_HR(hr);

        // ソースノードとレンダラーノードを接続
        hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
        CHECK_HR(hr);

        pSourceNode->Release();
        pOutputNode->Release();
    }

    pSD->Release();
    if (pRendererActivate) pRendererActivate->Release();
    return hr;
}

//---------------------------------------------------------
// トポロジの作成関数
//---------------------------------------------------------
HRESULT CreatePlaybackTopology(IMFMediaSource* pSource, HWND hwnd, IMFTopology** ppTopology)
{
    IMFTopology* pTopology = NULL;
    IMFPresentationDescriptor* pPD = NULL;
    HRESULT hr = S_OK;

    // トポロジを作成
    hr = MFCreateTopology(&pTopology);
    CHECK_HR(hr);

    // プレゼンテーションディスクリプタの取得
    hr = pSource->CreatePresentationDescriptor(&pPD);
    CHECK_HR(hr);

    // 各ストリームに対してノードを作成し、トポロジに追加
    DWORD cStreams = 0;
    hr = pPD->GetStreamDescriptorCount(&cStreams);
    CHECK_HR(hr);

    for (DWORD i = 0; i < cStreams; i++)
    {
        hr = AddStreamToTopology(pTopology, pSource, pPD, i, hwnd);
        CHECK_HR(hr);
    }

    *ppTopology = pTopology;
    pPD->Release();
    return hr;
}

//---------------------------------------------------------
// ムービー再生関数
//---------------------------------------------------------
HRESULT PlayVideo(HWND hwnd, const wchar_t* videoFilePath)
{
    IMFMediaSession* pSession = NULL;
    IMFMediaSource* pSource = NULL;
    IMFTopology* pTopology = NULL;
    IMFSourceResolver* pResolver = NULL;
    IMFMediaEvent* pEvent = NULL; // ここで pEvent を定義
    MediaEventType meType; // ここで meType を定義
    HRESULT hr = S_OK;

    // Media Foundation の初期化
    hr = MFStartup(MF_VERSION);
    CHECK_HR(hr);

    // ソースリゾルバーの作成
    hr = MFCreateSourceResolver(&pResolver);
    CHECK_HR(hr);

    // メディアソースの作成
    MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;
    IUnknown* pSourceUnk = NULL;
    hr = pResolver->CreateObjectFromURL
    (
        videoFilePath,                    // 再生する動画ファイルのパス
        MF_RESOLUTION_MEDIASOURCE,        // メディアソースを取得
        NULL,
        &ObjectType,
        &pSourceUnk
    );
    CHECK_HR(hr);

    // ソースの取得
    hr = pSourceUnk->QueryInterface(IID_PPV_ARGS(&pSource));
    pSourceUnk->Release();
    CHECK_HR(hr);

    // メディアセッションの作成
    hr = MFCreateMediaSession(nullptr, &pSession);
    if (FAILED(hr) || pSession == nullptr)
    {
        return hr;
    }

    // トポロジの作成
    hr = CreatePlaybackTopology(pSource, hwnd, &pTopology);
    CHECK_HR(hr);

    // トポロジをセッションに設定
    hr = pSession->SetTopology(0, pTopology);
    if (FAILED(hr) || pTopology == nullptr)
    {
        return hr;
    }

    if (pSession == nullptr)
    {
        return hr;
    }

    // 再生を 0 秒から開始する場合
    PROPVARIANT varStart;
    PropVariantInit(&varStart);
    varStart.vt = VT_EMPTY; // 0 秒からの再生を指示

    hr = pSession->Start(nullptr, &varStart);  // 第2引数に varStart を渡す
    PropVariantClear(&varStart);

    if (FAILED(hr)) 
    {
        return hr;
    }

    // イベントループで再生状態を監視
    while (true)
    {
        hr = pSession->GetEvent(0, &pEvent);
        CHECK_HR(hr);

        hr = pEvent->GetType(&meType);  // ここで meType を使用
        CHECK_HR(hr);

        if (meType == MESessionEnded) 
        {
            break;  // 再生が終了したらループを抜ける
        }

        pEvent->Release();
    }

    // セッションの終了
    hr = pSession->Close();
    CHECK_HR(hr);

    // メディアセッションの終了確認
    hr = pSession->GetEvent(0, &pEvent);
    CHECK_HR(hr);

    hr = pEvent->GetType(&meType);  // ここでも meType を使用
    CHECK_HR(hr);

    if (meType == MESessionClosed) 
    {
        // セッションが終了した
    }

    pEvent->Release();

    // リソース解放
    if (pSource) pSource->Shutdown();
    if (pSession) pSession->Shutdown();
    if (pResolver) pResolver->Release();
    if (pTopology) pTopology->Release();
    if (pSession) pSession->Release();
    if (pSource) pSource->Release();

    // Media Foundation の終了
    MFShutdown();
    return hr;
}