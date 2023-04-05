#include <criterion/criterion.h>
#include <echo.h>
#include <cmath>

Test(misc, echo) {
    echo("Test");
}

Test(misc, echo2) {
    echo2();
}

Test(misc, passing) {
    cr_assert(1);
}

Test(misc, pi) {
    cr_assert(fabs(M_PI -3.141) < 0.01);
}
