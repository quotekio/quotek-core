#include "matrixutils.hpp"

namespace quotek {

  namespace core {
    
    namespace utils {

		void removeColumn(Eigen::MatrixXd& matrix, unsigned int colToRemove) {
		    unsigned int numRows = matrix.rows();
		    unsigned int numCols = matrix.cols()-1;

		    if( colToRemove < numCols )
		        matrix.block(0,colToRemove,numRows,numCols-colToRemove) = matrix.block(0,colToRemove+1,numRows,numCols-colToRemove);

		    matrix.conservativeResize(numRows,numCols);
		}
	}
  }
}