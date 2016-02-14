#include "stdafx.h"
#include "Animation.h"


Animation::Animation(void)
{
}


Animation::~Animation(void)
{
}

bool Animation::LoadAnimation(const std::string& filename) {
	return true;
}

void Animation::Update(unsigned int elapsedTimeInMillis) {
}

const Animation::FrameSkeleton& Animation::GetSkeleton() const {
	return mAnimatedSkeleton;
}

int Animation::GetBoneCount() const {
	return mBoneCount;
}

const Animation::BoneInfo& Animation::GetBoneInfo(unsigned int index) const {
	return mBoneInfos[index];
}

void Animation::BuildFrameSkeleton(FrameSkeletonList& skeletons, const BoneInfoList& boneInfos, const BaseFrameList& baseFrames, const FrameData& frameData) {
}

void Animation::InterpolateSkeletons(FrameSkeleton& result, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float blendWeight) {
}
