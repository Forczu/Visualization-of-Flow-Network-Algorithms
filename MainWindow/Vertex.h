#pragma once

/// <summary>
/// Wierzcho³ek grafu.
/// </summary>
class Vertex
{
	short _id;
	
public:
	Vertex(short id);
	Vertex(Vertex const & v);
	~Vertex();

	Vertex * clone() const { return new Vertex(*this); }

#pragma region Getters & Setters
	inline short Id() const { return _id; }
	void Id(short val) { _id = val; }
#pragma endregion


#pragma region Operators
	inline bool operator==(Vertex const & other) const
	{
		Id() == other.Id() ? true : false;
	}
#pragma endregion

};

