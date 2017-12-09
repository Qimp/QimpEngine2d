#ifndef __MATH_LINEAR_CURVE_H__
#define __MATH_LINEAR_CURVE_H__

QIMP_BEGIN_NAMESPACE
//#include "CQuaternion.h"
//#include "CVector3.h"

template<class T>
struct LinearKeyframeTpl
{
	LinearKeyframeTpl(){};
	LinearKeyframeTpl(float& time, T& value):Time(time),Value(value){};

	int		Time;
	T		Value;

	friend bool operator < (const LinearKeyframeTpl& lhs, const LinearKeyframeTpl& rhs) { return lhs.Time < rhs.Time; }
	friend bool operator < (LinearKeyframeTpl<T> const& lhs, float t) { return lhs.Time < t; }
	friend bool operator < (float t, LinearKeyframeTpl<T> const& rhs) { return t < rhs.Time; }


};

template<class T>
class LinearCurveTpl
{
public:
	typedef LinearKeyframeTpl< T > Keyframe;
	
	typedef std::vector<Keyframe> KeyframeContainer;
	typedef typename KeyframeContainer::iterator iterator;
	typedef typename KeyframeContainer::const_iterator const_iterator;

public:
	T				Evaluate (int time);


	bool			IsValid () const							{ return Keyframes.size () >= 1; }
	int				GetLength() const;
	void			AddKey (float time, T value);
	void			AddKey (const Keyframe& key);
	void			PushKey (const Keyframe& key)				{ Keyframes.push_back (key); }
	const Keyframe&	GetKey (int index) const					{ assert(index >= 0 && index < Keyframes.size()); return Keyframes[index]; }
	Keyframe&		GetKey (int index)							{ assert(index >= 0 && index < Keyframes.size()); return const_cast<Keyframe&> (Keyframes[index]); }

	iterator		Begin ()									{ return Keyframes.begin (); }
	iterator		End ()										{ return Keyframes.end (); }
	const_iterator	Begin () const								{ return Keyframes.begin (); }
	const_iterator	End () const								{ return Keyframes.end (); }

	size_t			GetKeyCount () const						{ return Keyframes.size(); }
	void			RemoveKeys (iterator begin, iterator end);

public:
	KeyframeContainer	Keyframes;
};

typedef LinearCurveTpl<Quaternionf>		LinearCurveQuat;
typedef LinearCurveTpl<float>			LinearCurveFloat;
//typedef LinearCurveTpl<Vector2f>		CurveVec2;
typedef LinearCurveTpl<Vector3f>		LinearCurveVec3;
//typedef LinearCurveTpl<Vector4f>		CurveVec4;



template<class T>
T LinearCurveTpl<T>::Evaluate(int time)
{
	//currentTime = wrapTime(currentTime);
	if (Keyframes.size() == 0)
		return T();

	iterator iter = std::lower_bound(Keyframes.begin(), Keyframes.end(), (float)time);
	if (iter == Keyframes.end())
		return (*(--iter)).Value;
	Keyframe rhs = *iter;

	if (iter == Keyframes.begin())
		return rhs.Value;

	Keyframe lhs = *(--iter);

	float length = (float)(rhs.Time - lhs.Time);
	float percent = (time - lhs.Time) / length;
	percent = percent >= 1.0f ? 1.0f : percent;

	//	return lhs.Value * (1.0f - percent) + rhs.Value * percent; 
	return Slerp(lhs.Value, rhs.Value, percent);
}

template<class T>
void LinearCurveTpl<T>::RemoveKeys(iterator begin, iterator end)
{
	Keyframes.erease(begin, end);
}

template<class T>
void LinearCurveTpl<T>::AddKey(float time, T value)
{
	iterator iter = std::lower_bound(Keyframes.begin(), Keyframes.end(), time);
	Keyframes.insert(iter, Keyframe(time, value));
}

template<class T>
void LinearCurveTpl<T>::AddKey(const Keyframe& key)
{
	iterator iter = std::lower_bound(Keyframes.begin(), Keyframes.end(), key.time);
	Keyframes.insert(iter, key);
}

template<class T>
int LinearCurveTpl<T>::GetLength() const
{
	if (Keyframes.size() == 0)
		return 0;

	return Keyframes.rbegin()->Time;
}


QIMP_END_NAMESPACE

#endif//__MATH_LINEAR_CURVE_H__
