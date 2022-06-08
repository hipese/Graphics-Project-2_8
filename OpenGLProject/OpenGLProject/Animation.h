#pragma once
#include "Model.h"

class Animation
{
private:
	// 애니메이션에 포함된 모델의 배열입니다.
	Model** m_models = nullptr;
	// 애니메이션의 최대 프레임 수 입니다.
	int m_maxFrame = 0;
	// 애니메이션 렌더링 시 현재 프레임입니다.
	int m_currentFrame = 0;
	// 애니메이션의 반복 여부입니다.
	bool m_bLoop = false;
	// 애니메이션을 재생 설정입니다.
	int m_playing = 0;
	// 애니메이션의 재생 방향을 저장합니다.
	bool m_bIsReverse = false;
	// 애니메이션이 끝난 후 딜레이입니다.
	int m_delay = 0;
	int m_currentDelay = 0;
	// 애니메이션의 종료 여부입니다.
	bool m_bEndPlay = false;

public:
	Animation();
	virtual ~Animation();

	inline Model** GetModels() { return m_models; }
	inline Model* GetModel() { return m_models[m_currentFrame]; }
	// 애니메이션을 반복하려면 true로 설정하세요.
	inline void SetbLoop(int bLoop) { m_bLoop = bLoop; }
	// 애니메이션을 재생합니다.
	inline void Play() { m_playing = 1; InitFrame(false); }
	// 애니메이션을 역재생합니다.
	inline void ReversePlay() { m_playing = -1;  InitFrame(true); }
	// 애니메이션을 정지합니다.
	inline void Stop() { m_playing = 0; InitFrame(m_bIsReverse); }
	// 애니메이션을 일시정지합니다.
	inline void Parse() { m_playing = 0; }
	inline bool IsReverse() { return m_bIsReverse; }
	inline bool IsEndPlay() { return m_bEndPlay; }
	inline bool IsPlaying() { return m_playing != 0; }
	inline int GetPlaying() { return m_playing; }
	inline int GetMaxFrame() { return m_maxFrame; }
	inline void SetDelay(int delay) { m_delay = delay; }

	// Model을 파일로부터 불러옵니다.
	void LoadModel(const char* files, int maxFrame, Transform* parent = nullptr);
	// 프레임을 초기화합니다.
	void InitFrame(bool bIsReverse = true);
	// 이전 프레임으로 설정합니다.
	void PrevFrame();
	// 다음 프레임으로 설정합니다.
	void NextFrame();
	// 현재 프레임인 모델을 그립니다.
	void Draw(class Shader& shader);
	// 업데이트 호출 시 사용하세요.
	void Update();

};