#pragma once
#include <Windows.h>
#include <XAudio2.h>
#include <stdexcept>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

class XAudio2
{
private:
	IXAudio2* m_xaudio2 = nullptr;
	IXAudio2MasteringVoice* m_master_voice = nullptr;
	WAVEFORMATEXTENSIBLE m_wfx = { 0 };
	XAUDIO2_BUFFER m_buffer = { 0 };
	DWORD m_chunk_size;
	DWORD m_chunk_position;

private:
	bool create_xaudio2_device();
	bool create_xaudio2_mastering_voice_obj();
	HRESULT find_chunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT read_chunk_data(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

public:
	XAudio2();
	~XAudio2();

	IXAudio2SourceVoice* load_audio_data(LPCWSTR fileName);
	bool play_audio(IXAudio2SourceVoice* pSourceVoice);
	bool stop_audio(IXAudio2SourceVoice* pSourceVoice);
};