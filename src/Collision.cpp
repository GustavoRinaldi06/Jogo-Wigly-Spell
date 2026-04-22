#include "Collision.h"
#include <cmath>
#include <algorithm>

// Produto escalar
static inline float DotProduct(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

// Gera os 4 vértices do retângulo rotacionado
static void GetRotatedCorners(const Rect &box, float angle, Vec2 corners[4])
{
    Vec2 center = box.GetCenter();

    // pega os vertices da box
    corners[0] = Vec2(box.x, box.y);
    corners[1] = Vec2(box.x + box.w, box.y);
    corners[2] = Vec2(box.x + box.w, box.y + box.h);
    corners[3] = Vec2(box.x, box.y + box.h);

    // rotaciona para o angulo atual
    for (int i = 0; i < 4; ++i)
    {
        corners[i] = (corners[i] - center).Rotate(angle) + center;
    }
}

// Projeta os 4 pontos num eixo e obtém o intervalo [min, max]
static void ProjectOntoAxis(const Vec2 corners[4], const Vec2 &axis, float &min, float &max)
{
    min = max = DotProduct(corners[0], axis);
    for (int i = 1; i < 4; ++i)
    {
        float projection = DotProduct(corners[i], axis);
        if (projection < min)
            min = projection;
        if (projection > max)
            max = projection;
    }
}

// Função principal de verificar colisão
bool IsColliding(Rect &a, float angleOfA, Rect &b, float angleOfB)
{
    Vec2 cornersA[4], cornersB[4];
    GetRotatedCorners(a, angleOfA, cornersA);
    GetRotatedCorners(b, angleOfB, cornersB);

    // 4 eixos para testar: 2 de A e 2 de B
    Vec2 axes[4] = {
        (cornersA[1] - cornersA[0]).Normalize(),
        (cornersA[3] - cornersA[0]).Normalize(),
        (cornersB[1] - cornersB[0]).Normalize(),
        (cornersB[3] - cornersB[0]).Normalize()};

    // Para cada eixo
    for (int i = 0; i < 4; ++i)
    {
        float minA, maxA, minB, maxB;

        ProjectOntoAxis(cornersA, axes[i], minA, maxA);
        ProjectOntoAxis(cornersB, axes[i], minB, maxB);

        // Teste de separação
        if (maxA < minB || maxB < minA)
        {
            return false; // Separação encontrada → não há colisão
        }
    }

    // Eixos se sobrepõem → colisão
    return true;
}
