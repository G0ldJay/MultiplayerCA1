//Jason Lynch - D00137655
//Dylan Reilly D00194504
template<typename T>

std::string toString(const T& value)
{
	std::stringstream stream;
	stream << value;
	return stream.str();
}
