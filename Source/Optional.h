#pragma once

namespace mag
{
    template <typename T>
    class Optional final
    {
    public:
        Optional();

        void operator=(T value);

        inline bool HasValue() const
        {
            if (_elementExists)
            {
                return true;
            }

            return false;
        }

        inline T GetValue() const
        {
            return _element;
        }

    private:
        T _element;
        bool _elementExists;
    };

    template <typename T>
    Optional<T>::Optional() : 
        _elementExists(false)
    {

    }

    template <typename T>
    void Optional<T>::operator=(T value)
    {
        _element = value;
        _elementExists = true;
        return;
    }
}
