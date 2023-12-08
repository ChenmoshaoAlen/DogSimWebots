#pragma once
/* Includes ------------------------------------------------------------------*/
#include <../../libraries/eigen/Eigen/Eigen>
#include <cmath>
/* Private macros ------------------------------------------------------------*/
Eigen::MatrixXd H(20, 5);
Eigen::MatrixXd Fs(5, 1);
static const double L, L1, L2, s;/* TODO */
static const double errorMin = 0.001;
/* Private type --------------------------------------------------------------*/
namespace vmcspace
{
  template <typename T>
  const T &abs(const T &input)
  {
    return input < (T)0 ? -input : input;
  }
}
/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
 * @brief 弹簧阻尼器参数
 * 
 */
typedef struct _Spring_Damper
{
  double xd;
  double yd;
  double zd;
  double alphad;
  double betad;
  double xcurr;
  double ycurr;
  double zcurr;
  double alphacurr;
  double betacurr;
  double diffxd;        // 目标速度旋量
  double diffyd;        // 目标速度旋量
  double diffzd;        // 目标速度旋量
  double diffalphad;    // 目标速度旋量
  double diffbetad;     // 目标速度旋量
  double diffxcurr;     // 当前速度旋量
  double diffycurr;     // 当前速度旋量
  double diffzcurr;     // 当前速度旋量
  double diffalphacurr; // 当前速度旋量
  double diffbetacurr;  // 当前速度旋量
  double Kx;            // 弹簧参数
  double Ky;            // 弹簧参数
  double Kz;            // 弹簧参数
  double Kalpha;        // 弹簧参数
  double Kbeta;         // 弹簧参数
  double Bx;            // 阻尼器参数
  double By;            // 阻尼器参数
  double Bz;            // 阻尼器参数
  double Balpha;        // 阻尼器参数
  double Bbeta;         // 阻尼器参数
}Spring_Damper;
/* Exported function declarations --------------------------------------------*/

/**
 * @brief 映射矩阵内容初始化
 * 
 * @param _H 映射矩阵
 * @param alpha pitch偏角
 * @param beta roll偏角
 */
void MatrixHInit(Eigen::MatrixXd &_H, const double alpha, const double beta, const double O1lf, const double O2lf, const double O4lf, const double O1rf, const double O2rf, const double O4rf, const double O1lb, const double O2lb, const double O4lb, const double O1rb, const double O2rb, const double O4rb)
{
  static double Ltlf, Ltrf, Ltlb, Ltrb;
  static double Pl, Pr, Ql, Qr, Rl, Sl, Rr, Sr, Uf, Wf, Ub, Wb;
  static double O0lf, O0rf, O0lb, O0rb;
  static double O3lf, O3rf, O3lb, O3rb;

  O0lf = alpha - O1lf - O2lf;
  O0rf = alpha - O1rf - O2rf;
  O0lb = alpha - O1lb - O2lb;
  O0rb = alpha - O1rb - O2rb;
  O3lf = beta - O4lf;
  O3rf = beta - O4rf;
  O3lb = beta - O4lb;
  O3rb = beta - O4rb;

  Ltlf = L1 * cos(O0lf) + L2 * cos(O0lf + O1lf);
  Ltrf = L1 * cos(O0rf) + L2 * cos(O0rf + O1rf);
  Ltlb = L1 * cos(O0lb) + L2 * cos(O0lb + O1lb);
  Ltrb = L1 * cos(O0rb) + L2 * cos(O0rb + O1rb);

  Pl = -L1 * cos(O0lf) - L2 * cos(O0lf + O1lf) + L * sin(O0lf + O1lf + O2lf);
  Pr = -L1 * cos(O0rf) - L2 * cos(O0rf + O1rf) + L * sin(O0rf + O1rf + O2rf);
  Ql = -L1 * sin(O0lf) - L2 * sin(O0lf + O1lf) - L * cos(O0lf + O1lf + O2lf);
  Qr = -L1 * sin(O0rf) - L2 * sin(O0rf + O1rf) - L * cos(O0rf + O1rf + O2rf);
  Rl = -L1 * cos(O0lb) - L2 * cos(O0lb + O1lb) - L * sin(O0lb + O1lb + O2lb);
  Rr = -L1 * cos(O0rb) - L2 * cos(O0rb + O1rb) - L * sin(O0rb + O1rb + O2rb);
  Sl = -L1 * sin(O0lb) - L2 * sin(O0lb + O1lb) + L * cos(O0lb + O1lb + O2lb);
  Sr = -L1 * sin(O0rb) - L2 * sin(O0rb + O1rb) + L * cos(O0rb + O1rb + O2rb);
  Uf = s * sin(O3lf + O4lf) - Ltlf * cos(O3lf);
  Ub = s * sin(O3lb + O4lb) - Ltlb * cos(O3lb);
  Wf = -s * sin(O3rf + O4rf) - Ltrf * cos(O3rf);
  Wb = -s * sin(O3rb + O4rb) - Ltrb * cos(O3rb);

  _H << ((Ql*Qr*Rl*Sr + Ql*Qr*Rr*Sl)*(L2*cos(O0lf + O1lf) + L1*cos(O0lf) - L*sin(O0lf + O1lf + O2lf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pl*Qr*Rl*Sr + Pl*Qr*Rr*Sl)*(L2*sin(O0lf + O1lf) + L*cos(O0lf + O1lf + O2lf) + L1*sin(O0lf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0, (Ql*Qr*Sl*Sr*(L2*cos(O0lf + O1lf) + L1*cos(O0lf) - L*sin(O0lf + O1lf + O2lf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Pl*Qr*Sl*Sr*(L2*sin(O0lf + O1lf) + L*cos(O0lf + O1lf + O2lf) + L1*sin(O0lf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl), ((L2*cos(O0lf + O1lf) + L1*cos(O0lf) - L*sin(O0lf + O1lf + O2lf))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*sin(O0lf + O1lf) + L*cos(O0lf + O1lf + O2lf) + L1*sin(O0lf))*(Pl*Qr*Sl + Pl*Qr*Sr + Pl*Sl*Sr - Pr*Sl*Sr + Qr*Rl*Sr + Qr*Rr*Sl))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                      ((Ql*Qr*Rl*Sr + Ql*Qr*Rr*Sl)*(L2*cos(O0lf + O1lf) - L*sin(O0lf + O1lf + O2lf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pl*Qr*Rl*Sr + Pl*Qr*Rr*Sl)*(L2*sin(O0lf + O1lf) + L*cos(O0lf + O1lf + O2lf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                               (Ql*Qr*Sl*Sr*(L2*cos(O0lf + O1lf) - L*sin(O0lf + O1lf + O2lf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Pl*Qr*Sl*Sr*(L2*sin(O0lf + O1lf) + L*cos(O0lf + O1lf + O2lf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                               ((L2*cos(O0lf + O1lf) - L*sin(O0lf + O1lf + O2lf))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*sin(O0lf + O1lf) + L*cos(O0lf + O1lf + O2lf))*(Pl*Qr*Sl + Pl*Qr*Sr + Pl*Sl*Sr - Pr*Sl*Sr + Qr*Rl*Sr + Qr*Rr*Sl))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                                                                    - (L*cos(O0lf + O1lf + O2lf)*(Pl*Qr*Rl*Sr + Pl*Qr*Rr*Sl))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - (L*sin(O0lf + O1lf + O2lf)*(Ql*Qr*Rl*Sr + Ql*Qr*Rr*Sl))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                                                                             - (L*Pl*Qr*Sl*Sr*cos(O0lf + O1lf + O2lf))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (L*Ql*Qr*Sl*Sr*sin(O0lf + O1lf + O2lf))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                                                                               1/4 - (L*sin(O0lf + O1lf + O2lf)*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - (L*cos(O0lf + O1lf + O2lf)*(Pl*Qr*Sl + Pl*Qr*Sr + Pl*Sl*Sr - Pr*Sl*Sr + Qr*Rl*Sr + Qr*Rr*Sl))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl),                                                                                                                                                                                       0,
                                                                                                                                                                                                                                                                                                                                      0,   (Ub*Wb*Wf*(s*sin(O3lf + O4lf) - Lt*cos(O3lf)))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) - (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,   ((s*sin(O3lf + O4lf) - Lt*cos(O3lf))*(Ub*Wf + Wb*Wf))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) - (Ub*Uf*Wf + Uf*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
                                                                                                                                                                                                                                                                                                                                      0,                    (Ub*Wb*Wf*s*sin(O3lf + O4lf))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) - (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,                    (s*sin(O3lf + O4lf)*(Ub*Wf + Wb*Wf))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) - (Ub*Uf*Wf + Uf*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
        ((Ql*Qr*Rl*Sr + Ql*Qr*Rr*Sl)*(L2*cos(O0rf + O1rf) + L1*cos(O0rf) - L*sin(O0rf + O1rf + O2rf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pr*Ql*Rl*Sr + Pr*Ql*Rr*Sl)*(L2*sin(O0rf + O1rf) + L*cos(O0rf + O1rf + O2rf) + L1*sin(O0rf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0, (Ql*Qr*Sl*Sr*(L2*cos(O0rf + O1rf) + L1*cos(O0rf) - L*sin(O0rf + O1rf + O2rf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Pr*Ql*Sl*Sr*(L2*sin(O0rf + O1rf) + L*cos(O0rf + O1rf + O2rf) + L1*sin(O0rf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl), ((L2*cos(O0rf + O1rf) + L1*cos(O0rf) - L*sin(O0rf + O1rf + O2rf))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*sin(O0rf + O1rf) + L*cos(O0rf + O1rf + O2rf) + L1*sin(O0rf))*(Pr*Ql*Sl + Pr*Ql*Sr - Pl*Sl*Sr + Ql*Rl*Sr + Ql*Rr*Sl + Pr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                      ((Ql*Qr*Rl*Sr + Ql*Qr*Rr*Sl)*(L2*cos(O0rf + O1rf) - L*sin(O0rf + O1rf + O2rf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pr*Ql*Rl*Sr + Pr*Ql*Rr*Sl)*(L2*sin(O0rf + O1rf) + L*cos(O0rf + O1rf + O2rf)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                               (Ql*Qr*Sl*Sr*(L2*cos(O0rf + O1rf) - L*sin(O0rf + O1rf + O2rf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Pr*Ql*Sl*Sr*(L2*sin(O0rf + O1rf) + L*cos(O0rf + O1rf + O2rf)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                               ((L2*cos(O0rf + O1rf) - L*sin(O0rf + O1rf + O2rf))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*sin(O0rf + O1rf) + L*cos(O0rf + O1rf + O2rf))*(Pr*Ql*Sl + Pr*Ql*Sr - Pl*Sl*Sr + Ql*Rl*Sr + Ql*Rr*Sl + Pr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                                                                    - (L*cos(O0rf + O1rf + O2rf)*(Pr*Ql*Rl*Sr + Pr*Ql*Rr*Sl))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - (L*sin(O0rf + O1rf + O2rf)*(Ql*Qr*Rl*Sr + Ql*Qr*Rr*Sl))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                                                                             - (L*Pr*Ql*Sl*Sr*cos(O0rf + O1rf + O2rf))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (L*Ql*Qr*Sl*Sr*sin(O0rf + O1rf + O2rf))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                                                                               1/4 - (L*sin(O0rf + O1rf + O2rf)*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - (L*cos(O0rf + O1rf + O2rf)*(Pr*Ql*Sl + Pr*Ql*Sr - Pl*Sl*Sr + Ql*Rl*Sr + Ql*Rr*Sl + Pr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl),                                                                                                                                                                                       0,
                                                                                                                                                                                                                                                                                                                                      0, - (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) - (Ub*Uf*Wb*(s*sin(O3rf + O4rf) + Lt*cos(O3rf)))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0, - (Ub*Uf*Wf + Uf*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) - ((s*sin(O3rf + O4rf) + Lt*cos(O3rf))*(Ub*Uf + Uf*Wb))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
                                                                                                                                                                                                                                                                                                                                      0,                  - (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) - (Ub*Uf*Wb*s*sin(O3rf + O4rf))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,                  - (Ub*Uf*Wf + Uf*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) - (s*sin(O3rf + O4rf)*(Ub*Uf + Uf*Wb))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
        ((Pl*Qr*Rl*Sr + Pr*Ql*Rl*Sr)*(L2*sin(O0lb + O1lb) - L*cos(O0lb + O1lb + O2lb) + L1*sin(O0lb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr)*(L2*cos(O0lb + O1lb) + L1*cos(O0lb) + L*sin(O0lb + O1lb + O2lb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0, (Ql*Qr*Rl*Sr*(L2*sin(O0lb + O1lb) - L*cos(O0lb + O1lb + O2lb) + L1*sin(O0lb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Ql*Qr*Sl*Sr*(L2*cos(O0lb + O1lb) + L1*cos(O0lb) + L*sin(O0lb + O1lb + O2lb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl), ((L2*sin(O0lb + O1lb) - L*cos(O0lb + O1lb + O2lb) + L1*sin(O0lb))*(Ql*Qr*Rl + Pl*Qr*Sr + Pr*Ql*Sr - Ql*Qr*Rr + Ql*Rl*Sr + Qr*Rl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*cos(O0lb + O1lb) + L1*cos(O0lb) + L*sin(O0lb + O1lb + O2lb))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                      ((Pl*Qr*Rl*Sr + Pr*Ql*Rl*Sr)*(L2*sin(O0lb + O1lb) - L*cos(O0lb + O1lb + O2lb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr)*(L2*cos(O0lb + O1lb) + L*sin(O0lb + O1lb + O2lb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                               (Ql*Qr*Rl*Sr*(L2*sin(O0lb + O1lb) - L*cos(O0lb + O1lb + O2lb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Ql*Qr*Sl*Sr*(L2*cos(O0lb + O1lb) + L*sin(O0lb + O1lb + O2lb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                               ((L2*sin(O0lb + O1lb) - L*cos(O0lb + O1lb + O2lb))*(Ql*Qr*Rl + Pl*Qr*Sr + Pr*Ql*Sr - Ql*Qr*Rr + Ql*Rl*Sr + Qr*Rl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*cos(O0lb + O1lb) + L*sin(O0lb + O1lb + O2lb))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                                                                    - (L*cos(O0lb + O1lb + O2lb)*(Pl*Qr*Rl*Sr + Pr*Ql*Rl*Sr))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - (L*sin(O0lb + O1lb + O2lb)*(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                                                                             - (L*Ql*Qr*Rl*Sr*cos(O0lb + O1lb + O2lb))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (L*Ql*Qr*Sl*Sr*sin(O0lb + O1lb + O2lb))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                                                                               1/4 - (L*sin(O0lb + O1lb + O2lb)*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - (L*cos(O0lb + O1lb + O2lb)*(Ql*Qr*Rl + Pl*Qr*Sr + Pr*Ql*Sr - Ql*Qr*Rr + Ql*Rl*Sr + Qr*Rl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl),                                                                                                                                                                                       0,
                                                                                                                                                                                                                                                                                                                                      0,   (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) - (Uf*Wb*Wf*(s*sin(O3lb + O4lb) - Lt*cos(O3lb)))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,   (Ub*Uf*Wb + Ub*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) - ((s*sin(O3lb + O4lb) - Lt*cos(O3lb))*(Uf*Wb + Wb*Wf))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
                                                                                                                                                                                                                                                                                                                                      0,                    (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) - (Uf*Wb*Wf*s*sin(O3lb + O4lb))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,                    (Ub*Uf*Wb + Ub*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) - (s*sin(O3lb + O4lb)*(Uf*Wb + Wb*Wf))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
        ((Pl*Qr*Rr*Sl + Pr*Ql*Rr*Sl)*(L2*sin(O0rb + O1rb) - L*cos(O0rb + O1rb + O2rb) + L1*sin(O0rb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr)*(L2*cos(O0rb + O1rb) + L1*cos(O0rb) + L*sin(O0rb + O1rb + O2rb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0, (Ql*Qr*Rr*Sl*(L2*sin(O0rb + O1rb) - L*cos(O0rb + O1rb + O2rb) + L1*sin(O0rb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Ql*Qr*Sl*Sr*(L2*cos(O0rb + O1rb) + L1*cos(O0rb) + L*sin(O0rb + O1rb + O2rb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl), ((L2*sin(O0rb + O1rb) - L*cos(O0rb + O1rb + O2rb) + L1*sin(O0rb))*(Pl*Qr*Sl + Pr*Ql*Sl - Ql*Qr*Rl + Ql*Qr*Rr + Ql*Rr*Sl + Qr*Rr*Sl))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*cos(O0rb + O1rb) + L1*cos(O0rb) + L*sin(O0rb + O1rb + O2rb))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                      ((Pl*Qr*Rr*Sl + Pr*Ql*Rr*Sl)*(L2*sin(O0rb + O1rb) - L*cos(O0rb + O1rb + O2rb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - ((Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr)*(L2*cos(O0rb + O1rb) + L*sin(O0rb + O1rb + O2rb)))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                               (Ql*Qr*Rr*Sl*(L2*sin(O0rb + O1rb) - L*cos(O0rb + O1rb + O2rb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (Ql*Qr*Sl*Sr*(L2*cos(O0rb + O1rb) + L*sin(O0rb + O1rb + O2rb)))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                               ((L2*sin(O0rb + O1rb) - L*cos(O0rb + O1rb + O2rb))*(Pl*Qr*Sl + Pr*Ql*Sl - Ql*Qr*Rl + Ql*Qr*Rr + Ql*Rr*Sl + Qr*Rr*Sl))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - ((L2*cos(O0rb + O1rb) + L*sin(O0rb + O1rb + O2rb))*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) + 1/4,                                                                                                                                                                                       0,
                                                                                    - (L*cos(O0rb + O1rb + O2rb)*(Pl*Qr*Rr*Sl + Pr*Ql*Rr*Sl))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl) - (L*sin(O0rb + O1rb + O2rb)*(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr))/(2*Pl*Qr*Sl*Sr + 2*Pr*Ql*Sl*Sr - 2*Ql*Qr*Rl*Sr - 2*Ql*Qr*Rr*Sl),                                                                                                                                                        0,                                                                             - (L*Ql*Qr*Rr*Sl*cos(O0rb + O1rb + O2rb))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl) - (L*Ql*Qr*Sl*Sr*sin(O0rb + O1rb + O2rb))/(Pl*Qr*Sl*Sr + Pr*Ql*Sl*Sr - Ql*Qr*Rl*Sr - Ql*Qr*Rr*Sl),                                                                               1/4 - (L*sin(O0rb + O1rb + O2rb)*(Ql*Qr*Sl + Ql*Qr*Sr + Ql*Sl*Sr + Qr*Sl*Sr))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl) - (L*cos(O0rb + O1rb + O2rb)*(Pl*Qr*Sl + Pr*Ql*Sl - Ql*Qr*Rl + Ql*Qr*Rr + Ql*Rr*Sl + Qr*Rr*Sl))/(4*Pl*Qr*Sl*Sr + 4*Pr*Ql*Sl*Sr - 4*Ql*Qr*Rl*Sr - 4*Ql*Qr*Rr*Sl),                                                                                                                                                                                       0,
                                                                                                                                                                                                                                                                                                                                      0,   (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) + (Ub*Uf*Wf*(s*sin(O3rb + O4rb) + Lt*cos(O3rb)))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,   (Ub*Uf*Wb + Ub*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) + ((s*sin(O3rb + O4rb) + Lt*cos(O3rb))*(Ub*Uf + Ub*Wf))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf),
                                                                                                                                                                                                                                                                                                                                      0,                    (Ub*Uf*Wb*Wf)/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf) + (Ub*Uf*Wf*s*sin(O3rb + O4rb))/(Ub*Uf*Wb - Ub*Uf*Wf + Ub*Wb*Wf - Uf*Wb*Wf),                                                                                                                                                                                                                                                                               0,                                                                                                                                                                                                                                                                                                                                                                                           0,                    (Ub*Uf*Wb + Ub*Wb*Wf)/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf) + (s*sin(O3rb + O4rb)*(Ub*Uf + Ub*Wf))/(2*Ub*Uf*Wb - 2*Ub*Uf*Wf + 2*Ub*Wb*Wf - 2*Uf*Wb*Wf);
}





/**
 * @brief 力矩计算
 *
 * @param _H
 * @param lfTorque
 * @param rfTorque
 * @param lbTorque
 * @param rfTorque
 */
void TorqueCalculate(Eigen::MatrixXd &_H, Eigen::MatrixXd &_F, const Spring_Damper &sdPara, double lfTorque[4], double rfTorque[4], double lbTorque[4], double rbTorque[4])
{
  static Eigen::MatrixXd torMat(20, 1);

  static Eigen::MatrixXd KMat = Eigen::MatrixXd::Zero(5, 5), BMat = Eigen::MatrixXd::Zero(5, 5), Pose(5, 1), Veloc(5, 1);

  KMat(0, 0) = sdPara.Kx;
  KMat(1, 1) = sdPara.Ky;
  KMat(2, 2) = sdPara.Kz;
  KMat(3, 3) = sdPara.Kalpha;
  KMat(4, 4) = sdPara.Kbeta;
  BMat(0, 0) = sdPara.Bx;
  BMat(1, 1) = sdPara.By;
  BMat(2, 2) = sdPara.Bz;
  BMat(3, 3) = sdPara.Balpha;
  BMat(4, 4) = sdPara.Bbeta;
  Pose << (vmcspace::abs(sdPara.xd - sdPara.xcurr) > errorMin ? sdPara.xd - sdPara.xcurr : 0),
          (vmcspace::abs(sdPara.yd - sdPara.ycurr) > errorMin ? sdPara.yd - sdPara.ycurr : 0),
          (vmcspace::abs(sdPara.zd - sdPara.zcurr) > errorMin ? sdPara.zd - sdPara.zcurr : 0),
          (vmcspace::abs(sdPara.alphad - sdPara.alphacurr) > errorMin ? sdPara.alphad - sdPara.alphacurr : 0),
          (vmcspace::abs(sdPara.betad - sdPara.betacurr) > errorMin ? sdPara.betad - sdPara.betacurr : 0);
  Veloc << (vmcspace::abs(sdPara.diffxd - sdPara.diffxcurr) > errorMin ? sdPara.diffxd - sdPara.diffxcurr : 0),
           (vmcspace::abs(sdPara.diffyd - sdPara.diffycurr) > errorMin ? sdPara.diffyd - sdPara.diffycurr : 0),
           (vmcspace::abs(sdPara.diffzd - sdPara.diffzcurr) > errorMin ? sdPara.diffzd - sdPara.diffzcurr : 0),
           (vmcspace::abs(sdPara.diffalphad - sdPara.diffalphacurr) > errorMin ? sdPara.diffalphad - sdPara.diffalphacurr : 0),
           (vmcspace::abs(sdPara.diffbetad - sdPara.diffbetacurr) > errorMin ? sdPara.diffbetad - sdPara.diffbetacurr : 0);

  _F = KMat * Pose + BMat * Veloc;

  torMat = _H * _F;
  
  lfTorque[(int)JointEnumdef::LegDown] = torMat(1, 0);
  lfTorque[(int)JointEnumdef::LegUp] = torMat(2, 0);
  lfTorque[(int)JointEnumdef::Shoulder] = torMat(4, 0);
  rfTorque[(int)JointEnumdef::LegDown] = torMat(1+5, 0);
  rfTorque[(int)JointEnumdef::LegUp] = torMat(2+5, 0);
  rfTorque[(int)JointEnumdef::Shoulder] = torMat(4+5, 0);
  lbTorque[(int)JointEnumdef::LegDown] = torMat(1+10, 0);
  lbTorque[(int)JointEnumdef::LegUp] = torMat(2+10, 0);
  lbTorque[(int)JointEnumdef::Shoulder] = torMat(4+10, 0);
  rbTorque[(int)JointEnumdef::LegDown] = torMat(1+15, 0);
  rbTorque[(int)JointEnumdef::LegUp] = torMat(2+15, 0);
  rbTorque[(int)JointEnumdef::Shoulder] = torMat(4+15, 0);
  
}

