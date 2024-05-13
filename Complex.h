#pragma once

class complex
{
  private:
    double Re;
    double Im;

  public:
    complex()
    {
        Re = 0;
        Im = 0;
    }

    complex(double real, double image)
    {
        Re = real;
        Im = image;
    }

    complex(const complex &itemForCopy)
    {
        Re = itemForCopy.Re;
        Im = itemForCopy.Im;
    }

    double GetRe()
    {
        return Re;
    }

    double GetIm()
    {
        return Im;
    }

    complex operator+(complex item)
    {
        double real = this->Re + item.Re;
        double image = this->Im + item.Im;
        complex result(real, image);
        return result;
    }

    complex operator-(complex item)
    {
        double real = this->Re - item.Re;
        double image = this->Im - item.Im;
        complex result(real, image);
        return result;
    }

    complex operator*(complex item)
    {
        double real = this->Re;
        double image = this->Im;
        double realRes = real * item.Re - image * item.Im;
        double imageRes = real * item.Im + image * item.Re;
        complex result(realRes, imageRes);
        return result;
    }

    complex operator/(complex item)
    {
        double real = this->Re;
        double image = this->Im;
        double realRes = (real * item.Re + image * item.Im) / ((item.Re * (item.Re)) + item.Im * item.Im);
        double imageRes = (image * item.Re - real * item.Im) / ((item.Re * (item.Re)) + item.Im * item.Im);
        complex result(realRes, imageRes);
        return result;
    }

    friend std::istream &operator>>(std::istream &in, complex &item)
    {
        std::cout << "Enter real part of complex number ";
        in >> item.Re;
        std::cout << "Enter image part of complex number ";
        in >> item.Im;
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const complex &item)
    {
        if (item.Im < 0)
        {
            return out << item.Re << "+i" << "(" << item.Im << ")" << " ";
        }
        else
        {
            return out << item.Re << "+i" << item.Im << " ";
        }
    }

    complex &operator+=(complex item)
    {
        this->Re += item.Re;
        this->Im += item.Im;
        return *this;
    }

    complex &operator-=(complex item)
    {
        double real = this->Re;
        double image = this->Im;
        this->Re = real - item.Re;
        this->Im = image - item.Im;
        return *this;
    }

    complex &operator*=(complex item)
    {
        double real = this->Re;
        double image = this->Im;
        this->Re = real * item.Re - image * item.Im;
        this->Im = real * item.Im + image * item.Re;
        return *this;
    }

    complex &operator/=(complex item)
    {
        double real = this->Re;
        double image = this->Im;
        this->Re = (real * item.Re + image * item.Im) / ((item.Re) * (item.Re) + item.Im * item.Im);
        this->Im = (image * item.Re - real * item.Im) / ((item.Re) * (item.Re) + item.Im * item.Im);
        return *this;
    }

    bool operator==(complex item)
    {
        if (this->Re == item.Re && this->Im == item.Im)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator!=(complex item)
    {
        if (this->Re != item.Re || this->Im != item.Im)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

