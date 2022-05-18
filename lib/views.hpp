
#include <ranges>
#include <iterator>

namespace ViewDetails{
    template<std::ranges::forward_range Range>
    class _CircularView : public std::ranges::view_interface<_CircularView<Range>> {
        using RangeIteratorType = std::ranges::iterator_t<Range>;
        class CircularIterator : public RangeIteratorType{
            RangeIteratorType firstIt{};
            RangeIteratorType lastIt{};
        public:
            constexpr CircularIterator() = default;
            constexpr CircularIterator(Range& range)
            : RangeIteratorType{range.begin()}, firstIt{range.begin()}, lastIt{range.end()}{}

            constexpr bool operator==(std::unreachable_sentinel_t){return false;}

            constexpr auto& operator++(){
                RangeIteratorType::operator++();
                if(static_cast<RangeIteratorType>(*this) == lastIt){
                    RangeIteratorType::operator=(firstIt);
                }
                return *this;
            }

            constexpr auto operator++(int){
                CircularIterator temp{*this};
                ++*this;
                return temp;
            }

            constexpr auto& operator+=(long i){
                i%=std::distance(firstIt, lastIt);
                if(i > std::distance(static_cast<RangeIteratorType>(*this), lastIt) ){
                    RangeIteratorType::operator=(firstIt+i-std::distance(static_cast<RangeIteratorType>(*this), lastIt));
                }
                else{
                    RangeIteratorType::operator+=(i);
                }
                return *this;
            }
                return *this;
            }
        };

        CircularIterator iter;
    public:
        constexpr _CircularView() = default;
        constexpr _CircularView(Range range)
        : iter{range}
        {}

        constexpr CircularIterator begin() const {return iter; }
        constexpr auto             end()   const {return std::unreachable_sentinel; }
    };


    struct CircleViewAdaptor{
        template<std::ranges::viewable_range Range>
        constexpr auto operator()(Range&& r) const
        {
            return _CircularView<Range>(std::forward<Range>(r));
        }
    };
}


template <typename Range>
inline constexpr bool std::ranges::enable_borrowed_range<ViewDetails::_CircularView<Range>> = true;



namespace views{
    ViewDetails::CircleViewAdaptor circle;
}

template<std::ranges::viewable_range Range>
constexpr auto operator|(Range&& r, const ViewDetails::CircleViewAdaptor& circleView)
{
    return circleView(std::forward<Range>(r)) ;
}