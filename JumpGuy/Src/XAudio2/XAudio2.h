#pragma once

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

class XAudio2
{
private:
	IXAudio2* _pXAudio2 = nullptr;
	IXAudio2MasteringVoice* _pMasterVoice = nullptr;
	WAVEFORMATEXTENSIBLE _wfx = { 0 };
	XAUDIO2_BUFFER _buffer = { 0 };
	DWORD _dwChunkSize;
	DWORD _dwChunkPosition;

public:
	XAudio2()
	{
		if (!CreateXAudio2Device())
			MessageBoxA(NULL, "Failed to create XAudio2 device!", NULL, NULL);

		if (!CreateIXAudio2MasteringVoiceObj())
			MessageBoxA(NULL, "Failed to create a voice!", NULL, NULL);
	}
	~XAudio2()
	{
		delete _pXAudio2;
		delete _pMasterVoice;
	}

public:
	IXAudio2SourceVoice* LoadAudioData(LPCWSTR fileName);
	bool StartAudio(IXAudio2SourceVoice* pSourceVoice);
	bool StopAudio(IXAudio2SourceVoice* pSourceVoice);

private:
	bool CreateXAudio2Device();
	bool CreateIXAudio2MasteringVoiceObj();
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
};