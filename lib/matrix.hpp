#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>


namespace Matrix{

    template<class T>
    using Position = Utilities::Position<T>;

    template<class T>
    class Matrix{
        size_t n{0};
        size_t m{0};
        std::vector<T> matrix{};

        constexpr void checkRowBound(const size_t i) const {
            if(i>=n) throw std::out_of_range("Row index too large. Got "
                                            + std::to_string(i)
                                            + ", expected index in intervall [0,"
                                            + std::to_string(n)
                                            + ")\n" );
        }

        constexpr void checkColBound(const size_t j) const {
            if(j>=m) throw std::out_of_range("Column index too large. Got "
                                            + std::to_string(j)
                                            + ", expected index in intervall [0,"
                                            + std::to_string(m)
                                            + ")\n" );
        }

        constexpr void checkBounds(const size_t i, const size_t j) const {
            checkRowBound(i);
            checkColBound(j);
        }

        constexpr void fillResizedMatrix(const size_t nn, const size_t mm, std::vector<T>&& newMatrix){
            auto newMatrixIt = std::begin(newMatrix);
            auto matrixIt = std::begin(matrix);
            const auto min = std::min(m, mm);
            while(matrixIt != std::end(matrix) && newMatrixIt != std::end(newMatrix)){
                auto j=0u;
                while(j < min){
                    *newMatrixIt++ = std::move(*matrixIt++);
                    j++;
                }
                if(m > mm){
                    std::advance(matrixIt, m-mm);
                }
                else{
                    std::advance(newMatrixIt, mm-m);
                }
            }

            std::swap(matrix, newMatrix);
            n=nn;
            m=mm;
        }

    public:
        constexpr Matrix() = default;
        constexpr Matrix(const size_t _n, const size_t _m) : n{_n}, m{_m}, matrix(_n*_m){}
        constexpr Matrix(const size_t _n, const size_t _m, const T& _value) : n{_n}, m{_m}, matrix(_n*_m, _value){}

        template<std::forward_iterator I, std::sentinel_for<I> S>
        requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>& >
        constexpr Matrix(const size_t _n, const size_t _m, I _first, S _last) : n{_n}, m{_m}, matrix(_n*_m){
            assign(_first, _last);
        }

        template< std::ranges::forward_range R>
        constexpr Matrix(const size_t _n, const size_t _m, R&& _range) : n{_n}, m{_m}, matrix(_n*_m){
            assign(_range);
        }
        constexpr Matrix(const size_t _n, const size_t _m, std::initializer_list<T> _ilist) : n{_n}, m{_m}, matrix(_n*_m){
            assign(_ilist);
        }

        template<std::forward_iterator I, std::sentinel_for<I> S>
        requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>&>
        constexpr void assign( I first, S last, const size_t offset = 0u ){
            auto matrixIt = std::begin(matrix)+offset;
            while( first != last && matrixIt < std::end(matrix)){
                *matrixIt++ = *first++;
            }
        }

        template< std::ranges::forward_range R>
        constexpr void assign ( R&& range, const size_t offset = 0u ){
            assign(std::begin(range), std::end(range), offset);
        }

        constexpr void assign( std::initializer_list<T> ilist, const size_t offset = 0u ){
            assign(std::begin(ilist), std::end(ilist), offset);
        }

        std::vector<T>& data(){
            return matrix;
        }

        const std::vector<T>& data() const {
            return matrix;
        }

        constexpr std::vector<T>::reference operator()(const size_t i, const size_t j){
            checkBounds(i,j);
            return matrix[i*m+j];
        }

        constexpr const std::vector<T>::const_reference operator()(const size_t i, const size_t j) const {
            checkBounds(i,j);
            return matrix[i*m+j];
        }

        constexpr std::vector<T>::reference operator()(const Position<size_t> pos){
            const auto& [i,j] = pos;
            checkBounds(i,j);
            return matrix[i*m+j];
        }

        constexpr const std::vector<T>::const_reference operator()(const Position<size_t> pos) const {
            const auto& [i,j] = pos;
            checkBounds(i,j);
            return matrix[i*m+j];
        }

        constexpr std::vector<T>::reference operator[](const size_t longIndex){
            return matrix.at(longIndex);
        }

        constexpr const std::vector<T>::const_reference operator[](const size_t longIndex) const {
            return matrix.at(longIndex);
        }

        [[nodiscard]] constexpr auto   begin()       noexcept{ return matrix.  begin(); }
        [[nodiscard]] constexpr auto   begin() const noexcept{ return matrix.  begin(); }
        [[nodiscard]] constexpr auto  cbegin() const noexcept{ return matrix. cbegin(); }
        [[nodiscard]] constexpr auto   end  ()       noexcept{ return matrix.  end  (); }
        [[nodiscard]] constexpr auto   end  () const noexcept{ return matrix.  end  (); }
        [[nodiscard]] constexpr auto  cend  () const noexcept{ return matrix. cend  (); }
        [[nodiscard]] constexpr auto  rbegin()       noexcept{ return matrix. rbegin(); }
        [[nodiscard]] constexpr auto  rbegin() const noexcept{ return matrix. rbegin(); }
        [[nodiscard]] constexpr auto crbegin() const noexcept{ return matrix.crbegin(); }
        [[nodiscard]] constexpr auto  rend  ()       noexcept{ return matrix. rend  (); }
        [[nodiscard]] constexpr auto  rend  () const noexcept{ return matrix. rend  (); }
        [[nodiscard]] constexpr auto crend  () const noexcept{ return matrix.crend  (); }

        [[nodiscard]] constexpr auto size() const {
            return std::make_pair(n, m);
        }

        [[nodiscard]] constexpr size_t rows() const noexcept{
            return n;
        }

        [[nodiscard]] constexpr size_t cols() const noexcept{
            return m;
        }

        [[nodiscard]] constexpr auto row(const size_t i) {
            checkRowBound(i);
            return matrix | std::views::drop(i*m) | std::views::take(m);
        }

        [[nodiscard]] constexpr auto row(const size_t i) const {
            checkRowBound(i);
            return matrix | std::views::drop(i*m) | std::views::take(m);
        }

        [[nodiscard]] constexpr auto col(const size_t j) {
            checkColBound(j);
            auto mthElement = [m=this->m, i=0](const auto&) mutable { return i++%m==0; };
            return matrix | std::views::drop(j) | std::views::filter(mthElement);
        }

        [[nodiscard]] constexpr auto col(const size_t j) const {
            checkColBound(j);
            auto mthElement = [m=this->m, i=0](const auto&) mutable { return i++%m==0; };
            return matrix | std::views::drop(j) | std::views::filter(mthElement);
        }

        constexpr void resize(const size_t nn, const size_t mm){
            if(nn*mm == n*m){
                n = nn;
                m = mm;
                return;
            }
            std::vector<T> newMatrix(nn*mm);
            fillResizedMatrix(nn, mm, std::move(newMatrix));
        }

        constexpr void resize(const size_t nn, const size_t mm, const T& value){
            if(nn*mm == n*m){
                n = nn;
                m = mm;
                return;
            }
            std::vector<T> newMatrix(nn*mm, value);
            fillResizedMatrix(nn, mm, std::move(newMatrix));
        }

        void swap(Matrix<T>& newMatrix) noexcept {
            std::swap(matrix, newMatrix.data());
            std::swap(n, newMatrix.n);
            std::swap(m, newMatrix.m);
        }
    };


    /*
    for row and column index (i,j), returns it's valid adjacent neighbor indeces, not including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getNeighbors(const Matrix<T>& matrix, const S i, const S j){
        std::vector<Position<S>> neighbors{};
        if(i>0)               neighbors.emplace_back(Position{i-1, j  });
        if(i<matrix.rows()-1) neighbors.emplace_back(Position{i+1, j  });
        if(j>0)               neighbors.emplace_back(Position{i  , j-1});
        if(j<matrix.cols()-1) neighbors.emplace_back(Position{i  , j+1});
        return neighbors;
    }

    /*
    for row and column index n = i*matrix.columns+j, returns it's valid adjacent neighbor indeces, not including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getNeighbors(const Matrix<T>& matrix, const S longIndex){
        const auto m = matrix.cols();
        const auto i = longIndex/m;
        const auto j = longIndex%m;
        std::vector<S> neighbors{};
        if(i>0)               neighbors.emplace_back((i-1)*m+j  );
        if(i<matrix.rows()-1) neighbors.emplace_back((i+1)*m+j  );
        if(j>0)               neighbors.emplace_back((i  )*m+j-1);
        if(j<matrix.cols()-1) neighbors.emplace_back((i  )*m+j+1);
        return neighbors;
    }

    /*
    for row and column index pos(i,j), returns it's valid adjacent neighbor indeces, not including diagonals
    */
    template<class T, class S>
    auto getNeighbors(const Matrix<T>& matrix, const Position<S>& pos){
        const auto i = pos.first;
        const auto j = pos.second;
        return getNeighbors(matrix, i, j);
    }

    /*
    for row and column index *it=(i,j), returns it's valid adjacent neighbor indeces, including diagonals
    */
    template<class T, std::forward_iterator I>
    requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>& >
    auto getNeighbors(const Matrix<T>& matrix, I it){
        auto longIndex = std::ranges::distance(std::begin(matrix), it);
        return getNeighbors(matrix, longIndex);
    }

    /*
    for row and column index (i,j), returns it's valid adjacent neighbor indeces, including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getExtendedNeighbors(const Matrix<T>& matrix, const S i, const S j){
        std::vector<Position<S>> neighbors{getNeighbors(matrix, i, j)};
        if(i>0 && j>0)                             neighbors.emplace_back(Position{i-1, j-1});
        if(i<matrix.rows()-1 && j>0)               neighbors.emplace_back(Position{i+1, j-1});
        if(i>0               && j<matrix.cols()-1) neighbors.emplace_back(Position{i-1, j+1});
        if(i<matrix.rows()-1 && j<matrix.cols()-1) neighbors.emplace_back(Position{i+1, j+1});
        return neighbors;
    }

    /*
    for row and column index n = i*matrix.columns+j, returns it's valid adjacent neighbor indeces, including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getExtendedNeighbors(const Matrix<T>& matrix, const S longIndex){
        const auto m = matrix.cols();
        const auto i = longIndex/m;
        const auto j = longIndex%m;
        std::vector<S> neighbors{getNeighbors(matrix, longIndex)};
        if(i>0 && j>0)                             neighbors.emplace_back((i-1)*m+j-1);
        if(i<matrix.rows()-1 && j>0)               neighbors.emplace_back((i+1)*m+j-1);
        if(i>0               && j<matrix.cols()-1) neighbors.emplace_back((i-1)*m+j+1);
        if(i<matrix.rows()-1 && j<matrix.cols()-1) neighbors.emplace_back((i+1)*m+j+1);
        return neighbors;
    }

    /*
    for row and column index pos=(i,j), returns it's valid adjacent neighbor indeces, including diagonals
    */
    template<class T, class S>
    auto getExtendedNeighbors(const Matrix<T>& matrix, const Position<S>& pos){
        const auto i = pos.first;
        const auto j = pos.second;
        return getExtendedNeighbors(matrix, i, j);
    }

    /*
    for row and column index *it=(i,j), returns it's valid adjacent neighbor indeces, including diagonals
    */
    template<class T, std::forward_iterator I>
    requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>& >
    auto getExtendedNeighbors(const Matrix<T>& matrix, I it){
        const auto longIndex = (size_t) std::ranges::distance(std::begin(matrix), it);
        return getExtendedNeighbors(matrix, longIndex);
    }

    template<class T>
    void swap(Matrix<T>& rhs, Matrix<T>& lhs) noexcept {
        rhs.swap(lhs);
    }

    template<class T>
    void print(T&& v, const char separator = ' '){
        for(const auto& i : v){
            std::cout << i << separator;
        }
        std::cout << '\n';
    }

    template<class T>
    void print(Matrix<T>& m, const char columnSeparator = '\t') {
        for(auto i=0u; i<m.rows(); i++){
            print(m.row(i), columnSeparator);
        }
        std::cout << '\n';
    }

}

#endif