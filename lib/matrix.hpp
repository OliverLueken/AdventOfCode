#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <span>

#include "utilities.hpp"

namespace Matrix{

    template<class T>
    using Position = Utilities::Position<T>;

    template<class T>
    class Matrix{
        size_t _n{0};
        size_t _m{0};
        std::vector<T> matrix{};

        constexpr void checkRowBound(const size_t i) const {
            if(i>=_n) throw std::out_of_range("Row index too large. Got "
                                            + std::to_string(i)
                                            + ", expected index in intervall [0,"
                                            + std::to_string(_n)
                                            + ")\n" );
        }

        constexpr void checkColBound(const size_t j) const {
            if(j>=_m) throw std::out_of_range("Column index too large. Got "
                                            + std::to_string(j)
                                            + ", expected index in intervall [0,"
                                            + std::to_string(_m)
                                            + ")\n" );
        }

        constexpr void checkBounds(const size_t i, const size_t j) const {
            checkRowBound(i);
            checkColBound(j);
        }

        constexpr void fillResizedMatrix(const size_t nn, const size_t mm, std::vector<T>&& newMatrix){
            auto newMatrixIt = std::begin(newMatrix);
            auto matrixIt = std::begin(matrix);
            const auto min = std::min(_m, mm);
            while(matrixIt != std::end(matrix) && newMatrixIt != std::end(newMatrix)){
                auto j=0u;
                while(j < min){
                    *newMatrixIt++ = std::move(*matrixIt++);
                    j++;
                }
                if(_m > mm){
                    std::advance(matrixIt, _m-mm);
                }
                else{
                    std::advance(newMatrixIt, mm-_m);
                }
            }

            std::swap(matrix, newMatrix);
            _n=nn;
            _m=mm;
        }

    public:
        constexpr Matrix() = default;
        constexpr Matrix(const size_t __n, const size_t __m)                    : _n{__n}, _m{__m}, matrix(__n*__m){}
        constexpr Matrix(const size_t __n, const size_t __m, const T& _value)   : _n{__n}, _m{__m}, matrix(__n*__m, _value){}

        template< std::forward_iterator I, std::sentinel_for<I> S >
        requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>& >
        constexpr Matrix(const size_t __n, const size_t __m, I _first, S _last) : _n{__n}, _m{__m}, matrix(__n*__m){
            assign(_first, _last);
        }

        template< std::ranges::forward_range R>
        constexpr Matrix(const size_t __n, const size_t __m, R&& _range) : _n{__n}, _m{__m}, matrix(__n*__m){
            assign(_range);
        }
        constexpr Matrix(const size_t __n, const size_t __m, std::initializer_list<T> _ilist) : _n{__n}, _m{__m}, matrix(__n*__m){
            assign(_ilist);
        }

        template< std::forward_iterator I, std::sentinel_for<I> S >
        requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>&>
        constexpr void assign( I first, S last, const size_t offset = 0u ){
            if( static_cast<unsigned long>(std::distance(first, last)) > _n*_m ){
                last = first;
                std::advance(last, _n*_m);
            }
            std::ranges::copy(first, last, std::begin(matrix)+offset);
        }

        template< std::ranges::forward_range R >
        constexpr void assign( R&& range, const size_t offset = 0u ){
            assign(std::ranges::begin(range), std::ranges::end(range), offset);
        }

        constexpr void assign( std::initializer_list<T> ilist, const size_t offset = 0u ){
            assign(std::begin(ilist), std::end(ilist), offset);
        }

        constexpr       std::vector<T>& data()       noexcept {
            return matrix;
        }

        constexpr const std::vector<T>& data() const noexcept {
            return matrix;
        }

        constexpr       std::vector<T>::reference       operator()(const size_t i, const size_t j)       {
            checkBounds(i,j);
            return matrix[i*_m+j];
        }

        constexpr const std::vector<T>::const_reference operator()(const size_t i, const size_t j) const {
            checkBounds(i,j);
            return matrix[i*_m+j];
        }

        constexpr       std::vector<T>::reference       operator()(const Position<size_t> pos)       {
            const auto& [i,j] = pos;
            checkBounds(i,j);
            return matrix[i*_m+j];
        }

        constexpr const std::vector<T>::const_reference operator()(const Position<size_t> pos) const {
            const auto& [i,j] = pos;
            checkBounds(i,j);
            return matrix[i*_m+j];
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
            return std::make_pair(_n, _m);
        }

        [[nodiscard]] constexpr size_t rows() const noexcept{
            return _n;
        }

        [[nodiscard]] constexpr size_t cols() const noexcept{
            return _m;
        }

        template<class Iter>
        struct ColumnIteratorTemplate : public Iter{
            constexpr ColumnIteratorTemplate() = default;   //subrange needs this
            constexpr ColumnIteratorTemplate(Iter _it, size_t __m)
                : Iter{_it}, _m{__m} {}

            constexpr ColumnIteratorTemplate&  operator++()      { Iter::operator+=(_m); return *this; }
            constexpr ColumnIteratorTemplate   operator++(int)   { ColumnIteratorTemplate tmp = *this; ++(*this); return tmp; }
            constexpr ColumnIteratorTemplate&  operator+=(size_t j) { Iter::operator+=(j*_m); return *this; }

            constexpr ColumnIteratorTemplate& operator--()      { Iter::operator-=(_m); return *this; }
            constexpr ColumnIteratorTemplate  operator--(int)   { ColumnIteratorTemplate tmp = *this; --(*this); return tmp; }
            constexpr ColumnIteratorTemplate& operator-=(size_t j) { Iter::operator-=(j*_m); return *this; }
        private:
            size_t _m;
        };

        using             Iterator = std::vector<T>::              iterator;
        using        ConstIterator = std::vector<T>::        const_iterator;
        using      ReverseIterator = std::vector<T>::      reverse_iterator;
        using ConstReverseIterator = std::vector<T>::const_reverse_iterator;

        using             ColumnIterator = ColumnIteratorTemplate<            Iterator>;
        using        ConstColumnIterator = ColumnIteratorTemplate<       ConstIterator>;
        using      ReverseColumnIterator = ColumnIteratorTemplate<     ReverseIterator>;
        using ConstReverseColumnIterator = ColumnIteratorTemplate<ConstReverseIterator>;

        using             RowIterator =             Iterator;
        using        ConstRowIterator =        ConstIterator;
        using      ReverseRowIterator =      ReverseIterator;
        using ConstReverseRowIterator = ConstReverseIterator;

        [[nodiscard]] constexpr auto   columnBegin (size_t j)       noexcept { return ColumnIterator     (matrix. begin()+j, _m)   ; }
        [[nodiscard]] constexpr auto   columnEnd   (size_t j)       noexcept { return ColumnIterator     (matrix. end()  +j, _m)   ; }

        [[nodiscard]] constexpr auto   columnBegin (size_t j) const noexcept { return ConstColumnIterator(matrix.cbegin()+j, _m)   ; }
        [[nodiscard]] constexpr auto   columnEnd   (size_t j) const noexcept { return ConstColumnIterator(matrix.cend()  +j, _m)   ; }

        [[nodiscard]] constexpr auto  cColumnBegin (size_t j) const noexcept { return ConstColumnIterator(matrix.cbegin()+j, _m)   ; }
        [[nodiscard]] constexpr auto  cColumnEnd   (size_t j) const noexcept { return ConstColumnIterator(matrix.cend()  +j, _m)   ; }

        [[nodiscard]] constexpr auto   rowBegin    (size_t i)       noexcept { return RowIterator        (matrix. begin()+_m*i    ); }
        [[nodiscard]] constexpr auto   rowEnd      (size_t i)       noexcept { return RowIterator        (matrix. begin()+_m*(i+1)); }

        [[nodiscard]] constexpr auto   rowBegin    (size_t i) const noexcept { return ConstRowIterator   (matrix.cbegin()+_m*i    ); }
        [[nodiscard]] constexpr auto   rowEnd      (size_t i) const noexcept { return ConstRowIterator   (matrix.cbegin()+_m*(i+1)); }

        [[nodiscard]] constexpr auto  cRowBegin    (size_t i) const noexcept { return ConstRowIterator   (matrix.cbegin()+_m*i    ); }
        [[nodiscard]] constexpr auto  cRowEnd      (size_t i) const noexcept { return ConstRowIterator   (matrix.cbegin()+_m*(i+1)); }

        [[nodiscard]] constexpr auto  rColumnBegin (size_t j)       noexcept { return ReverseColumnIterator     (matrix. rbegin()+j, _m)   ; }
        [[nodiscard]] constexpr auto  rColumnEnd   (size_t j)       noexcept { return ReverseColumnIterator     (matrix. rend()  +j, _m)   ; }

        [[nodiscard]] constexpr auto  rColumnBegin (size_t j) const noexcept { return ConstReverseColumnIterator(matrix.crbegin()+j, _m)   ; }
        [[nodiscard]] constexpr auto  rColumnEnd   (size_t j) const noexcept { return ConstReverseColumnIterator(matrix.crend()  +j, _m)   ; }

        [[nodiscard]] constexpr auto crColumnBegin (size_t j) const noexcept { return ConstReverseColumnIterator(matrix.crbegin()+j, _m)   ; }
        [[nodiscard]] constexpr auto crColumnEnd   (size_t j) const noexcept { return ConstReverseColumnIterator(matrix.crend()  +j, _m)   ; }

        [[nodiscard]] constexpr auto  rRowBegin    (size_t i)       noexcept { return ReverseRowIterator        (matrix. rbegin()+_m*i    ); }
        [[nodiscard]] constexpr auto  rRowEnd      (size_t i)       noexcept { return ReverseRowIterator        (matrix. rbegin()+_m*(i+1)); }

        [[nodiscard]] constexpr auto  rRowBegin    (size_t i) const noexcept { return ConstReverseRowIterator   (matrix.crbegin()+_m*i    ); }
        [[nodiscard]] constexpr auto  rRowEnd      (size_t i) const noexcept { return ConstReverseRowIterator   (matrix.crbegin()+_m*(i+1)); }

        [[nodiscard]] constexpr auto crRowBegin    (size_t i) const noexcept { return ConstReverseRowIterator   (matrix.crbegin()+_m*i    ); }
        [[nodiscard]] constexpr auto crRowEnd      (size_t i) const noexcept { return ConstReverseRowIterator   (matrix.crbegin()+_m*(i+1)); }

        [[nodiscard]] constexpr auto col(const size_t j) {
            checkColBound(j);
            return std::ranges::subrange{columnBegin(j), columnEnd(j)};
        }

        [[nodiscard]] constexpr auto col(const size_t j) const {
            checkColBound(j);
            return std::ranges::subrange(cColumnBegin(j), cColumnEnd(j));
        }

        [[nodiscard]] constexpr auto reverseCol(const size_t j) {
            checkColBound(j);
            return std::ranges::subrange{rColumnBegin(j), rColumnEnd(j)};
        }

        [[nodiscard]] constexpr auto reverseCol(const size_t j) const {
            checkColBound(j);
            return std::ranges::subrange(crColumnBegin(j), crColumnEnd(j));
        }

        [[nodiscard]] constexpr auto row(const size_t i) {
            checkRowBound(i);
            return std::ranges::subrange{rowBegin(i), rowEnd(i)};
        }

        [[nodiscard]] constexpr auto row(const size_t i) const {
            checkRowBound(i);
            return std::ranges::subrange{cRowBegin(i), cRowEnd(i)};
        }

        [[nodiscard]] constexpr auto reverseRow(const size_t i) {
            checkRowBound(i);
            return std::ranges::subrange{rRowBegin(i), rRowEnd(i)};
        }

        [[nodiscard]] constexpr auto reverseRow(const size_t i) const {
            checkRowBound(i);
            return std::ranges::subrange{crRowBegin(i), crRowEnd(i)};
        }

        constexpr void resize(const size_t nn, const size_t mm){
            if(nn*mm == _n*_m){
                _n = nn;
                _m = mm;
                return;
            }
            std::vector<T> newMatrix(nn*mm);
            fillResizedMatrix(nn, mm, std::move(newMatrix));
        }

        constexpr void resize(const size_t nn, const size_t mm, const T& value){
            if(nn*mm == _n*_m){
                _n = nn;
                _m = mm;
                return;
            }
            std::vector<T> newMatrix(nn*mm, value);
            fillResizedMatrix(nn, mm, std::move(newMatrix));
        }

        void swap(Matrix<T>& newMatrix) noexcept {
            std::swap(matrix, newMatrix.data());
            std::swap(_n, newMatrix._n);
            std::swap(_m, newMatrix._m);
        }
    };


    /*
    for row and column index (i,j), returns its valid adjacent neighbor indices, not including diagonals
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
    for row and column index _n = i*matrix.columns+j, returns its valid adjacent neighbor indices, not including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getNeighbors(const Matrix<T>& matrix, const S longIndex){
        const auto _m = matrix.cols();
        const auto i = longIndex/_m;
        const auto j = longIndex%_m;
        return getNeighbors(matrix, i, j);
    }

    /*
    for row and column index pos(i,j), returns its valid adjacent neighbor indices, not including diagonals
    */
    template<class T, class S>
    auto getNeighbors(const Matrix<T>& matrix, const Position<S>& pos){
        const auto i = pos.first;
        const auto j = pos.second;
        return getNeighbors(matrix, i, j);
    }

    /*
    for row and column index *it=(i,j), returns its valid adjacent neighbor indices, including diagonals
    */
    template<class T, std::forward_iterator I>
    requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>& >
    auto getNeighbors(const Matrix<T>& matrix, I it){
        auto longIndex = std::ranges::distance(std::begin(matrix), it);
        return getNeighbors(matrix, longIndex);
    }

    /*
    for row and column index (i,j), returns its valid adjacent neighbor indices, including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getNeighborsIncludingDiagonals(const Matrix<T>& matrix, const S i, const S j){
        std::vector<Position<S>> neighbors{getNeighbors(matrix, i, j)};
        if(i>0 && j>0)                             neighbors.emplace_back(Position{i-1, j-1});
        if(i<matrix.rows()-1 && j>0)               neighbors.emplace_back(Position{i+1, j-1});
        if(i>0               && j<matrix.cols()-1) neighbors.emplace_back(Position{i-1, j+1});
        if(i<matrix.rows()-1 && j<matrix.cols()-1) neighbors.emplace_back(Position{i+1, j+1});
        return neighbors;
    }

    /*
    for row and column index _n = i*matrix.columns+j, returns its valid adjacent neighbor indices, including diagonals
    */
    template<class T, std::unsigned_integral S = uint32_t>
    auto getNeighborsIncludingDiagonals(const Matrix<T>& matrix, const S longIndex){
        const auto _m = matrix.cols();
        const auto i = longIndex/_m;
        const auto j = longIndex%_m;
        return getNeighborsIncludingDiagonals(matrix, i, j);
    }

    /*
    for row and column index pos=(i,j), returns its valid adjacent neighbor indices, including diagonals
    */
    template<class T, class S>
    auto getNeighborsIncludingDiagonals(const Matrix<T>& matrix, const Position<S>& pos){
        const auto i = pos.first;
        const auto j = pos.second;
        return getNeighborsIncludingDiagonals(matrix, i, j);
    }

    /*
    for row and column index *it=(i,j), returns its valid adjacent neighbor indices, including diagonals
    */
    template<class T, std::forward_iterator I>
    requires std::same_as< std::iter_value_t<I>&, std::iter_value_t<const T*>& >
    auto getNeighborsIncludingDiagonals(const Matrix<T>& matrix, I it){
        const auto longIndex = (size_t) std::ranges::distance(std::begin(matrix), it);
        return getNeighborsIncludingDiagonals(matrix, longIndex);
    }

    template<class T>
    void swap(Matrix<T>& rhs, Matrix<T>& lhs) noexcept {
        rhs.swap(lhs);
    }

    template<typename T>
    concept Container = requires (T c){c.begin();};

    template<typename T>
    concept Printable = requires (T x){ std::cout << x; };

    template<Printable T>
    void print(const T& value, const char separator = ' '){
        std::cout << value << separator;
    }

    template<Container C>
    auto print(const C& c, const char separator = ' '){
        std::ranges::for_each(c, [separator](const auto& c_){print(c_, separator);});
        std::cout << '\n';
    }

    template<typename T>
    void print(const Matrix<T>& _m, const char columnSeparator = ' ') {
        for(auto i=0u; i<_m.rows(); i++){
            print(_m.row(i), columnSeparator);
        }
        std::cout << '\n';
    }
}

#endif