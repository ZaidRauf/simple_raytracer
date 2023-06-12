struct Vector3 {
    Vector3();
    Vector3(const float a, const float b, const float c);
    float x = 0;
    float y = 0;
    float z = 0;
    Vector3 operator=(const Vector3 &other);
};

Vector3 operator+(const Vector3 &a, const Vector3 &b);

Vector3 operator-(const Vector3 &a, const Vector3 &b);

float operator*(const Vector3 &a, const Vector3 &b);

Vector3 operator*(const float s, const Vector3 &v);

Vector3 operator*(const Vector3 &v, const float s);