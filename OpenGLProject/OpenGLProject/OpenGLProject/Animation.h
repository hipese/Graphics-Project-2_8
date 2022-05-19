#pragma once
#include "Model.h"

class Animation
{
private:
	// 애니메이션에 포함된 모델의 배열입니다.
	Model** m_models = nullptr;
	// 애니메이션의 최대 프레임 수 입니다.
	int m_max_frame = 0;
	// 애니메이션 렌더링 시 현재 프레임입니다.
	int m_current_frame = 0;
	// 애니메이션의 반복 여부입니다.
	bool m_bLoop = false;
	// 애니메이션을 재생 설정입니다.
	int m_Playing = 0;
public:
	inline Model** GetModels() { return m_models; }
	// 애니메이션을 반복하려면 true로 설정하세요.
	inline void SetbLoop(int bLoop) { m_bLoop = bLoop; }
	// 애니메이션을 재생합니다.
	inline void Play() { m_Playing = 1; }
	// 애니메이션을 정지합니다.
	inline void Stop() { m_Playing = 0; m_current_frame = 0; }
	// 애니메이션을 일시정지합니다.
	inline void Parse() { m_Playing = 0; }
	// 애니메이션을 역재생합니다.
	inline void ReversePlay() { m_Playing = -1; }
	inline bool IsEndFrame() { return m_current_frame == m_max_frame - 1; }
public:
	Animation();
	virtual ~Animation();

	// Model을 파일로부터 불러옵니다.
	void LoadModel(const char* files, int max_frame, Transform* parent = nullptr);
	// 프레임을 초기화합니다.
	void InitFrame();
	// 이전 프레임으로 설정합니다.
	void PrevFrame();
	// 다음 프레임으로 설정합니다.
	void NextFrame();
	// 현재 프레임인 모델을 그립니다.
	void Draw(class Shader& shader);
	// 업데이트 호출 시 사용하세요.
	void Update();

};