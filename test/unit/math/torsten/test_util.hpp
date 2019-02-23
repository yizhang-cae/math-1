#ifndef STAN_MATH_TORSTEN_TEST_UTIL
#define STAN_MATH_TORSTEN_TEST_UTIL

#include <stan/math/rev/mat.hpp>
#include <gtest/gtest.h>
#include <test/unit/math/prim/arr/functor/harmonic_oscillator.hpp>
#include <test/unit/math/prim/arr/functor/lorenz.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace torsten {
  namespace test {
    /*
     * Test @c std::vector<var> results between two results. 
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     */
    template<typename T1, typename T2>
    void test_val(std::vector<std::vector<T1>>& y1,
                  std::vector<std::vector<T2>>& y2) {
      using stan::math::value_of;
      EXPECT_EQ(y1.size(), y2.size());
      for (size_t i = 0; i < y1.size(); ++i) {
        EXPECT_EQ(y1[i].size(), y2[i].size());
        for (size_t j = 0; j < y1[i].size(); ++j) {
          EXPECT_FLOAT_EQ(value_of(y1[i][j]), value_of(y2[i][j]));
        }
      }
    }

    /*
     * Test @c MatrixXd results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     */
    template<typename T1, typename T2>
    void test_val(Eigen::Matrix<T1, -1, -1>& y1,
                  Eigen::Matrix<T2, -1, -1>& y2) {
      using stan::math::value_of;
      EXPECT_EQ(y1.rows(), y2.rows());
      EXPECT_EQ(y1.cols(), y2.cols());
      for (int i = 0; i < y1.size(); ++i) {
        EXPECT_FLOAT_EQ(value_of(y1(i)), value_of(y2(i)));
      }
    }

    /*
     * Test @c MatrixXd results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param rtol relative tolerance
     * @param rtol absolute tolerance
     */
    template<typename T1, typename T2>
    void test_val(Eigen::Matrix<T1, -1, -1>& y1,
                  Eigen::Matrix<T2, -1, -1>& y2,
                  double rtol, double atol) {
      using stan::math::value_of;
      EXPECT_EQ(y1.rows(), y2.rows());
      EXPECT_EQ(y1.cols(), y2.cols());
      for (int i = 0; i < y1.size(); ++i) {
        double y1_i = value_of(y1(i));
        double y2_i = value_of(y2(i));
        if (abs(y1_i) < 1e-5 && abs(y2_i) < 1e-5) {
          EXPECT_NEAR(y1_i, y2_i, atol);
        } else {
          double err = std::max(y1_i, y2_i) * rtol;          
          EXPECT_NEAR(y1_i, y2_i, err);
        }
      }
    }

    /*
     * Test @c MatrixXd results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     */
    template<typename T1, typename T2>
    void test_val(Eigen::Matrix<T1, -1, 1>& y1,
                  Eigen::Matrix<T2, -1, 1>& y2) {
      using stan::math::value_of;
      EXPECT_EQ(y1.size(), y2.size());
      for (int i = 0; i < y1.size(); ++i) {
        EXPECT_FLOAT_EQ(value_of(y1(i)), value_of(y2(i)));
      }
    }

    /*
     * Test vector of vectors results against @c MatrixXd.
     *
     * @param y1 results in the form of vector of vectors
     * @param y2 @c MatrixXd results.
     */
    void test_val(std::vector<std::vector<double> > & y1,
                  Eigen::MatrixXd& y2) {
      EXPECT_EQ(y1.size(), y2.rows());
      for (size_t i = 0; i < y1.size(); ++i) {
        EXPECT_EQ(y1[i].size(), y2.cols());
        for (int j = 0; j < y2.cols(); ++j) {
          EXPECT_FLOAT_EQ(y1[i][j], y2(i, j));
        }
      }
    }

    /*
     * Test @c VectorXd results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     */
    void test_val(Eigen::VectorXd& y1, Eigen::VectorXd& y2) {
      EXPECT_EQ(y1.size(), y2.size());
      for (int i = 0; i < y1.size(); ++i) {
        EXPECT_FLOAT_EQ(y1(i), y2(i));
      }
    }

    /*
     * Test @c std::vector<var> results between two results. 
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta parameters regarding which the gradient
     *              would be taken and checked.
     * @param pk_y one result
     * @param stan_y the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta,
                   std::vector<std::vector<stan::math::var>>& pk_y,
                   std::vector<std::vector<stan::math::var>>& stan_y,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(pk_y.size(), stan_y.size());
      for (size_t i = 0; i < pk_y.size(); ++i) { 
        EXPECT_EQ(pk_y[i].size(), stan_y[i].size());
      }

      for (size_t i = 0; i < pk_y.size(); ++i) {
        for (size_t j = 0; j < pk_y[i].size(); ++j) {
          EXPECT_NEAR(pk_y[i][j].val(), stan_y[i][j].val(), fval_eps);
        }
      }

      std::vector<double> g, g1;
      for (size_t i = 0; i < pk_y.size(); ++i) {
        for (size_t j = 0; j < pk_y[i].size(); ++j) {
          stan::math::set_zero_all_adjoints();
          pk_y[i][j].grad(theta, g);
          stan::math::set_zero_all_adjoints();
          stan_y[i][j].grad(theta, g1);
          for (size_t m = 0; m < theta.size(); ++m) {
            EXPECT_NEAR(g[m], g1[m], sens_eps);
          }
        }
      }
    }

    /*
     * Test @c std::vector<var> results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta1 parameters regarding which the gradient
     *              would be taken by @c y1 and checked.
     * @param theta2 parameters regarding which the gradient
     *              would be taken by @c y2 and checked.
     * @param pk_y one result
     * @param stan_y the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta1,
                   std::vector<stan::math::var>& theta2,
                   std::vector<std::vector<stan::math::var>>& y1,
                   std::vector<std::vector<stan::math::var>>& y2,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(theta1.size(), theta2.size());
      EXPECT_EQ(y1.size(), y2.size());
      for (size_t i = 0; i < y1.size(); ++i) {
        EXPECT_EQ(y1[i].size(), y2[i].size());
      }

      for (size_t i = 0; i < y1.size(); ++i) {
        for (size_t j = 0; j < y1[i].size(); ++j) {
          EXPECT_NEAR(y1[i][j].val(), y2[i][j].val(), fval_eps);
        }
      }

      std::vector<double> g, g1;
      for (size_t i = 0; i < y1.size(); ++i) {
        for (size_t j = 0; j < y1[i].size(); ++j) {
          stan::math::set_zero_all_adjoints();
          y1[i][j].grad(theta1, g);
          stan::math::set_zero_all_adjoints();
          y2[i][j].grad(theta2, g1);
          for (size_t m = 0; m < theta1.size(); ++m) {
            EXPECT_NEAR(g[m], g1[m], sens_eps);
          }
        }
      }
    }

    /*
     * Test @c vector of @c var vectors against @c var matrix
     *
     * @param theta1 parameters regarding which the gradient
     *              would be taken by @c y1 and checked.
     * @param theta2 parameters regarding which the gradient
     *              would be taken by @c y2 and checked.
     * @param y1 result in form of vector of vectors
     * @param y2 result in form of matrix.
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta1,
                   std::vector<stan::math::var>& theta2,
                   std::vector<std::vector<stan::math::var>>& y1,
                   Eigen::Matrix<stan::math::var, -1, -1>& y2,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(theta1.size(), theta2.size());
      EXPECT_EQ(y1.size(), y2.rows());
      for (size_t i = 0; i < y1.size(); ++i) {
        EXPECT_EQ(y1[i].size(), y2.cols());
      }

      for (size_t i = 0; i < y1.size(); ++i) {
        for (size_t j = 0; j < y1[i].size(); ++j) {
          EXPECT_NEAR(y1[i][j].val(), y2(i, j).val(), fval_eps);
        }
      }

      std::vector<double> g, g1;
      for (size_t i = 0; i < y1.size(); ++i) {
        for (size_t j = 0; j < y1[i].size(); ++j) {
          stan::math::set_zero_all_adjoints();
          y1[i][j].grad(theta1, g);
          stan::math::set_zero_all_adjoints();
          y2(i, j).grad(theta2, g1);
          for (size_t m = 0; m < theta1.size(); ++m) {
            EXPECT_NEAR(g[m], g1[m], sens_eps);
          }
        }
      }
    }

    /*
     * Test @c std::vector<var> results between two results. 
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta parameters regarding which the gradient
     *              would be taken and checked.
     * @param pk_y one result
     * @param stan_y the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta,
                   Eigen::Matrix<stan::math::var, -1, -1>& pk_y,
                   Eigen::Matrix<stan::math::var, -1, -1>& stan_y,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(pk_y.rows(), stan_y.rows());
      EXPECT_EQ(pk_y.cols(), stan_y.cols());

      for (int i = 0; i < pk_y.size(); ++i) {
        EXPECT_NEAR(pk_y(i).val(), stan_y(i).val(), fval_eps);
      }

      std::vector<double> g, g1;
      for (int i = 0; i < pk_y.size(); ++i) {
        stan::math::set_zero_all_adjoints();
        pk_y(i).grad(theta, g);
        stan::math::set_zero_all_adjoints();
        stan_y(i).grad(theta, g1);
        for (size_t m = 0; m < theta.size(); ++m) {
          EXPECT_NEAR(g[m], g1[m], sens_eps);
        }
      }
    }

    /*
     * Test @c std::vector<var> results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta parameters regarding which the gradient
     *              would be taken and checked.
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c y1
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta,
                   stan::math::vector_v& y1,
                   stan::math::vector_v& y2,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(y1.size(), y2.size());

      for (int i = 0; i < y1.size(); ++i) {
        EXPECT_NEAR(y1(i).val(), y2(i).val(), fval_eps);
      }

      std::vector<double> g, g1;
      for (int i = 0; i < y1.size(); ++i) {
        stan::math::set_zero_all_adjoints();
        y1(i).grad(theta, g);
        stan::math::set_zero_all_adjoints();
        y2(i).grad(theta, g1);
        for (size_t m = 0; m < theta.size(); ++m) {
          EXPECT_NEAR(g[m], g1[m], sens_eps);
        }
      }
    }

    /*
     * Test @c std::vector<var> results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta1 parameters regarding which the gradient
     *               would be taken by @c y1 and checked.
     * @param theta2 parameters regarding which the gradient
     *               would be taken by @c y2 and checked.
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta1,
                   std::vector<stan::math::var>& theta2,
                   stan::math::vector_v& y1,
                   stan::math::vector_v& y2,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(y1.size(), y2.size());
      EXPECT_EQ(theta1.size(), theta2.size());

      for (int i = 0; i < y1.size(); ++i) {
        EXPECT_NEAR(y1(i).val(), y2(i).val(), fval_eps);
      }

      std::vector<double> g, g1;
      for (int i = 0; i < y1.size(); ++i) {
        stan::math::set_zero_all_adjoints();
        y1(i).grad(theta1, g);
        stan::math::set_zero_all_adjoints();
        y2(i).grad(theta2, g1);
        for (size_t m = 0; m < theta1.size(); ++m) {
          EXPECT_NEAR(g[m], g1[m], sens_eps);
        }
      }
    }

    /*
     * Test @c std::vector<var> results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta1 parameters regarding which the gradient
     *               would be taken by @c y1 and checked.
     * @param theta2 parameters regarding which the gradient
     *               would be taken by @c y2 and checked.
     * @param y1 one result
     * @param y2 the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta1,
                   Eigen::Matrix<stan::math::var, 1, -1>& theta2,
                   stan::math::vector_v& y1,
                   stan::math::vector_v& y2,
                   double fval_eps,
                   double sens_eps) {
      // grad() only accepts std::vector
      std::vector<stan::math::var> theta(theta2.size());
      Eigen::Matrix<stan::math::var, 1, -1>::Map(theta.data(), theta2.size()) = theta2;

      test_grad(theta1, theta, y1, y2, fval_eps, sens_eps);
    }

    /*
     * Test @c std::vector<var> results between two results.
     * An example use would be to have the results coming from torsten
     * and stan, respectively, so ensure the soundness of
     * torsten results.
     *
     * @param theta parameters regarding which the gradient
     *              would be taken and checked.
     * @param pk_y one result
     * @param stan_y the other result to be compared against
     *              with, must of same shape and size as to @c pk_y
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    void test_grad(std::vector<stan::math::var>& theta1,
                   std::vector<stan::math::var>& theta2,
                   Eigen::Matrix<stan::math::var, -1, -1>& y1,
                   Eigen::Matrix<stan::math::var, -1, -1>& y2,
                   double fval_eps,
                   double sens_eps) {
      EXPECT_EQ(theta1.size(), theta2.size());
      EXPECT_EQ(y1.rows(), y2.rows());
      EXPECT_EQ(y1.cols(), y2.cols());

      for (int i = 0; i < y1.size(); ++i) {
        EXPECT_NEAR(y1(i).val(), y2(i).val(), fval_eps);
      }

      std::vector<double> g, g1;
      for (int i = 0; i < y1.size(); ++i) {
        stan::math::set_zero_all_adjoints();
        y1(i).grad(theta1, g);
        stan::math::set_zero_all_adjoints();
        y2(i).grad(theta2, g1);
        for (size_t m = 0; m < theta1.size(); ++m) {
          EXPECT_NEAR(g[m], g1[m], sens_eps);
        }
      }
    }

    /*
     * test gradients against finite difference
     *
     * Given a functor that takes a single parameter vector,
     * compare the gradients of the functor w.r.t. the
     * vector parameter.
     *
     * @tparam F1 functor type that return data, must takes a single vector
     * @tparam F2 functor type that return @c var, must takes a single vector
     * argument and returns a matrix.
     * @param f functor
     * @param theta parameter of the functor's function
     * @param h step size when eval finite difference
     * @param fval_esp tolerance of values
     * @param sens_esp tolerance of gradients
     */
    template<typename F1, typename F2>
    void test_grad(F1& f1, F2& f2,
                   std::vector<double>& theta,
                   double h,
                   double fval_eps,
                   double r_sens_eps,  double a_sens_eps) {
      std::vector<stan::math::var> theta_v(stan::math::to_var(theta));
      Eigen::MatrixXd fd = f1(theta);
      Eigen::Matrix<stan::math::var,
                    Eigen::Dynamic,
                    Eigen::Dynamic> fv = f2(theta_v);

      std::vector<double> g;
      for (size_t i = 0; i < theta.size(); ++i) {
        std::vector<stan::math::var> p{theta_v[i]};
        std::vector<double> theta_h(theta);
        theta_h[i] += h;
        Eigen::MatrixXd fd_h = f1(theta_h);
        EXPECT_EQ(fv.size(), fd.size());
        for (int j = 0; j < fv.size(); ++j) {
          EXPECT_NEAR(fv(j).val(), fd(j), fval_eps);
          stan::math::set_zero_all_adjoints();
          fv(j).grad(p, g);
          double g_fd = (fd_h(j) - fd(j))/h;
          if (abs(g[0]) < 1e-5 && abs(g_fd) < 1e-5) {
            EXPECT_NEAR(g[0], g_fd, a_sens_eps);
          } else {
            EXPECT_NEAR(g[0], g_fd, r_sens_eps * std::max(abs(g[0]), abs(g_fd)));
          }
        }
      }
    }


  }
}

#endif
