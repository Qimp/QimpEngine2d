#ifndef __INT256_H__
#define __INT256_H__

QIMP_BEGIN_NAMESPACE

class Int256
{
public:
	int val[8];

public:
	void SetValue(int value)
	{
		val[0] = value;
		val[1] = value;
		val[2] = value;
		val[3] = value;
		val[4] = value;
		val[5] = value;
		val[6] = value;
		val[7] = value;
	}

	const Int256& operator = (const Int256& rhs)
	{
		val[0] = rhs.val[0];
		val[1] = rhs.val[1];
		val[2] = rhs.val[2];
		val[3] = rhs.val[3];
		val[4] = rhs.val[4];
		val[5] = rhs.val[5];
		val[6] = rhs.val[6];
		val[7] = rhs.val[7];

		return *this;
	}

	void And(Int256& rhs)
	{
		val[0] &= rhs.val[0];
		val[1] &= rhs.val[1];
		val[2] &= rhs.val[2];
		val[3] &= rhs.val[3];
		val[4] &= rhs.val[4];
		val[5] &= rhs.val[5];
		val[6] &= rhs.val[6];
		val[7] &= rhs.val[7];
	}

	void Or(Int256& rhs)
	{
		val[0] |= rhs.val[0];
		val[1] |= rhs.val[1];
		val[2] |= rhs.val[2];
		val[3] |= rhs.val[3];
		val[4] |= rhs.val[4];
		val[5] |= rhs.val[5];
		val[6] |= rhs.val[6];
		val[7] |= rhs.val[7];
	}

	void DelAllBit()
	{
		SetValue(0);
	}

	void AddBit(int i)
	{
		int count = i / 32;
		int offset = i % 32;
		val[count] |= (1 << offset);
	}

	void DelBit(int i)
	{
		int count = i / 32;
		int offset = i % 32;
		val[count] &= (1 << offset);
	}

	bool TestBit(int i)
	{
		int count = i / 32;
		int offset = i % 32;
		return ((val[count] & (1 << offset)) != 0);
	}

	void Inverse()
	{
		val[0] = ~val[0];
		val[1] = ~val[1];
		val[2] = ~val[2];
		val[3] = ~val[3];
		val[4] = ~val[4];
		val[5] = ~val[5];
		val[6] = ~val[6];
		val[7] = ~val[7];
	}

};

QIMP_END_NAMESPACE

#endif