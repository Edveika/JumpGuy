#include "../Includes.h"

bool XAudio2::CreateXAudio2Device()
{
	// Create xAudio2 device
	if (FAILED(XAudio2Create(&_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) return false;

	return true;
}

bool XAudio2::CreateIXAudio2MasteringVoiceObj()
{
	// Creates a IXAudio2MasteringVoice object
	if (FAILED(_pXAudio2->CreateMasteringVoice(&_pMasterVoice))) return false;

	return true;
}

HRESULT XAudio2::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT XAudio2::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	return hr;
}

IXAudio2SourceVoice* XAudio2::LoadAudioData(LPCWSTR fileName)
{
	HRESULT hr;
	IXAudio2SourceVoice* tmp;

	// Open the file
	HANDLE hFile = CreateFile
	(
		fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (INVALID_HANDLE_VALUE == hFile) return NULL;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN)) return NULL;

	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, _dwChunkSize, _dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), _dwChunkPosition);

	if (filetype != fourccWAVE) return NULL;

	FindChunk(hFile, fourccFMT, _dwChunkSize, _dwChunkPosition);
	ReadChunkData(hFile, &_wfx, _dwChunkSize, _dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, _dwChunkSize, _dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[_dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, _dwChunkSize, _dwChunkPosition);

	_buffer.AudioBytes = _dwChunkSize;  //size of the audio buffer in bytes
	_buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	_buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer

	if (FAILED(hr = _pXAudio2->CreateSourceVoice(&tmp, (WAVEFORMATEX*)&_wfx))) return NULL;

	if (FAILED(hr = tmp->SubmitSourceBuffer(&_buffer))) return NULL;

	return tmp;
}

bool XAudio2::StartAudio(IXAudio2SourceVoice* pSourceVoice)
{
	if (FAILED(pSourceVoice->Start(0))) return false;

	return true;
}

bool XAudio2::StopAudio(IXAudio2SourceVoice* pSourceVoice)
{
	if (FAILED(pSourceVoice->Stop(0))) return false;

	return false;
}