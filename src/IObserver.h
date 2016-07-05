template<class T>
class IObserver
{
public:
	virtual void Update(T* observable) = 0;
};