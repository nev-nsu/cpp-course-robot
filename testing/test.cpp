#include <fstream>
#include <vector>

#include "gtest/gtest.h"
#include "../Plane/PlanarSurface.h"
#include "../Solver.h"
#include "../utility/boost_setup.h"
#include "../Language/LangPoint.h"
#include "../Language/LangSurface.h"
#include "../utility/fsort.h"

TEST (solver, sort) {
    typedef std::tuple<int, double> t;
    typedef std::vector<t> array;
    array a = {std::make_tuple(1, 5),
               std::make_tuple(2, 8),
               std::make_tuple(3, 1)};
    array b = {std::make_tuple(3, 1),
               std::make_tuple(1, 5),
               std::make_tuple(2, 8)};
    auto cmp = [](t first, t second) {
        return get_dist(first) < get_dist(second);
    };
    std::sort(a.begin(), a.end(), cmp);
    ASSERT_EQ(a, b);
}

TEST (planar_point, cmp) {
    PlanarPoint a(0, 0);
    PlanarPoint b(0, 1);
    PlanarPoint c(1, 0);
    PlanarPoint d(1, 1);
    ASSERT_TRUE (a < b);
    ASSERT_TRUE (b < c);
    ASSERT_TRUE (c < d);
    ASSERT_TRUE (a == a);
}

TEST (planar_point, near) {
    PlanarPoint a(1, 1);
    std::vector<PlanarPoint> b = {PlanarPoint(0, 1), PlanarPoint(2, 1), PlanarPoint(1, 0), PlanarPoint(1, 2)};
    ASSERT_EQ (a.near(), b);
}

TEST (planar_point, dist) {
    PlanarPoint a(0, 0);
    PlanarPoint b(5, 0);
    PlanarPoint c(0, 5);
    PlanarPoint d(5, 5);
    ASSERT_EQ(a.dist(b), b.dist(a));
    ASSERT_EQ(a.dist(b, Topology::planar, 6), 5);
    ASSERT_EQ(a.dist(b, Topology::cylinder, 6), 1);
    ASSERT_EQ(a.dist(c, Topology::cylinder, 6), 5);
    ASSERT_EQ(a.dist(d, Topology::cylinder, 6), 6);
    ASSERT_EQ(a.dist(d, Topology::tor, 6, 6), 2);
}

TEST (planar_surface, reading) {
    ASSERT_THROW(PlanarSurface("map/in/map0.in"), BadMap);
    ASSERT_THROW(PlanarSurface("map/in/map1.in"), BadMap);
    ASSERT_NO_THROW(PlanarSurface("map/in/map2.in"));
}

TEST (planar_surface, lookup) {
    PlanarSurface a = PlanarSurface("map/in/map2.in");
    std::vector<std::tuple<PlanarPoint, double>> b = {std::make_tuple(PlanarPoint(0, 1), 7)};
    ASSERT_EQ (a.lookup(), b);
    ASSERT_EQ (a.move(PlanarPoint(0, 1)), 7);
    ASSERT_THROW(a.move(PlanarPoint(1, 1)), BadMove);
    ASSERT_THROW(a.move(PlanarPoint(3, 3)), BadMove);
}

TEST (planar_surface, finish) {
    PlanarSurface a("map/in/map3.in");
    ASSERT_NO_THROW(a.move(PlanarPoint(1, 0)));
    ASSERT_TRUE(a.done());
}

TEST (planar_surface, print) {
    std::ofstream out("map/out/test0.out");
    PlanarSurface a("map/in/map2.in");
    out << a;
    out.flush();
    ASSERT_TRUE(fcmp("map/out/test0.out", "map/ans/test0.ans"));
}

TEST (planar_solver, map2_planar) {
    PlanarSurface a("map/in/map2.in");
    Solver<PlanarPoint>(&a).solve();
    std::ofstream out("map/out/test1.out");
    out << a;
    out.flush();
    ASSERT_TRUE(fcmp("map/out/test1.out", "map/ans/test1.ans"));
}

TEST (planar_solver, map2_cylinder) {
    PlanarSurface a("map/in/map2.in", Topology::cylinder);
    Solver<PlanarPoint>(&a).solve();
    std::ofstream out("map/out/test2.out");
    out << a;
    out.flush();
    ASSERT_TRUE(fcmp("map/out/test2.out", "map/ans/test2.ans"));
}

TEST (planar_solver, map2_tor) {
    PlanarSurface a("map/in/map2.in", Topology::tor);
    Solver<PlanarPoint>(&a).solve();
    std::ofstream out("map/out/test3.out");
    out << a;
    out.flush();
    ASSERT_TRUE(fcmp("map/out/test3.out", "map/ans/test3.ans"));
}

TEST (planar_solver, no_way) {
    PlanarSurface a("map/in/map4.in");
    ASSERT_FALSE (Solver<PlanarPoint>(&a).solve());
}

TEST (lang_point, near) {
    LangPoint a("a", "b");
    std::vector<LangPoint> b = {LangPoint("b"),
                                LangPoint("ba"),
                                LangPoint("ab"),
                                LangPoint("")};
    ASSERT_EQ (a.near(), b);
}

TEST (lang_point, cmp) {
    ASSERT_TRUE (LangPoint("aaa") < LangPoint("bbb"));
    ASSERT_TRUE (LangPoint("aaa", "123") == LangPoint("aaa", "456"));
}

TEST (lang_surface, moving) {
    LangSurface a("dict.txt", "дт", "код", "кот");
    ASSERT_THROW(a.move(LangPoint("bugaga", "дт")), BadMove);
    ASSERT_THROW(a.move(LangPoint("кот", "д")), BadMove);
    ASSERT_NO_THROW(a.move(LangPoint("кот", "дт")));
    ASSERT_TRUE(a.done());
}

TEST (lang_solver, basic_test) {
    LangSurface a("dict.txt", "дт", "код", "кот");
    ASSERT_TRUE (Solver<LangPoint>(&a).solve());
    std::ofstream out("lang/out/test0.out");
    out << a;
    out.flush();
    ASSERT_TRUE (fcmp("lang/out/test0.out", "lang/ans/test0.ans"));
}

TEST (lang_solver, no_way) {
    LangSurface a("dict.txt", russian_alphabet, "вертолёт", "булочка");
    ASSERT_FALSE (Solver<LangPoint>(&a).solve(100));
    std::ofstream out("lang/out/test1.out");
    out << a;
    out.flush();
    ASSERT_TRUE (fcmp("lang/out/test1.out", "lang/ans/test1.ans"));

}

TEST (lang_solver, human_tests) {
    LangSurface temp("dict.txt", russian_alphabet, "слово", "слово");
    std::string a[] = {"урок", "паук", "кот", "река", "лупа", "поток", "мел", "соль"};
    std::string b[] = {"слон", "муха", "пёс", "гора", "море", "ворон", "соль", "пел"};
    for (int i = 0; i < 6; i++) {
        LangSurface c(temp, a[i], b[i]);
        ASSERT_FALSE (Solver<LangPoint>(&c).solve(2));
        ASSERT_TRUE (Solver<LangPoint>(&c).solve());
    }
}

TEST (solver, path) {
    LangSurface a("dict.txt", russian_alphabet, "кот", "пёс");
    Solver<LangPoint> b(&a);
    b.solve();
    auto c = b.get_path();
    ASSERT_EQ(std::string(c.front()), "кот");
    ASSERT_EQ(std::string(c.back()), "пёс");
}

TEST (crash_tests, lang_solver) {
    ASSERT_THROW(LangSurface("there-is-no-spoon", "", "", ""), BadMap);
    ASSERT_THROW(LangSurface("dict.txt", "", "", ""), BadMap);
    LangSurface *temp = nullptr;
    ASSERT_NO_THROW(temp = new LangSurface("dict.txt", "test", "слово", "слово"));
    if (temp) {
        ASSERT_THROW (temp->move(LangPoint("корова")), BadMove);
        ASSERT_THROW (temp->move(LangPoint("корова", "test")), BadMove);
        ASSERT_THROW (temp->move(LangPoint("слово")), BadMove);
        ASSERT_NO_THROW (temp->move(LangPoint("слово", "test")));
        Solver<LangPoint> b(temp);
        ASSERT_NO_THROW(b.solve());
        ASSERT_NO_THROW(b.solve(-10));
    }
}

int main(int argc, char **argv) {
    log_init("../../logs/robot_test.log");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}