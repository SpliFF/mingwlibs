//  Copyright (c) 2001-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_KARMA_CENTER_ALIGNMENT_FEB_27_2007_1216PM)
#define BOOST_SPIRIT_KARMA_CENTER_ALIGNMENT_FEB_27_2007_1216PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include <boost/spirit/home/support/component.hpp>
#include <boost/spirit/home/support/attribute_of.hpp>
#include <boost/spirit/home/karma/domain.hpp>
#include <boost/spirit/home/karma/directive/detail/center_alignment_generate.hpp>
#include <boost/lexical_cast.hpp>

///////////////////////////////////////////////////////////////////////////////
//
//  The BOOST_KARMA_DEFAULT_FIELD_LENGTH specifies the default field length
//  to be used for padding.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(BOOST_KARMA_DEFAULT_FIELD_LENGTH)
#define BOOST_KARMA_DEFAULT_FIELD_LENGTH 10
#endif

namespace boost { namespace spirit { namespace karma
{
    ///////////////////////////////////////////////////////////////////////////
    //  The simple center alignment directive is used for center[...]
    //  generators. It uses default values for the generated width (defined via
    //  the BOOST_KARMA_DEFAULT_FIELD_LENGTH constant) and for the padding
    //  generator (always spaces).
    ///////////////////////////////////////////////////////////////////////////
    struct simple_center_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::argument1<Component>::type,
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter, typename Parameter>
        static bool
        generate(Component const& component, OutputIterator& sink,
            Context& ctx, Delimiter const& d, Parameter const& param)
        {
            return detail::center_generate(sink, ctx, d, param,
                argument1(component), BOOST_KARMA_DEFAULT_FIELD_LENGTH,
                spirit::as_component(karma::domain(), ' '));
        }

        template <typename Component, typename Context>
        static std::string what(Component const& component, Context const& ctx)
        {
            std::string result = "center[";

            typedef typename
                spirit::result_of::argument1<Component>::type::director
            director;

            result += director::what(spirit::argument1(component), ctx);
            result += "]";
            return result;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //  The center alignment with width directive, is used for generators
    //  like center(width)[...]. It uses a default value for the padding
    //  generator (always spaces).
    ///////////////////////////////////////////////////////////////////////////
    struct width_center_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::subject<Component>::type,
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter, typename Parameter>
        static bool
        generate(Component const& component, OutputIterator& sink,
            Context& ctx, Delimiter const& d, Parameter const& param)
        {
            return detail::center_generate(sink, ctx, d, param,
                subject(component), proto::child_c<0>(argument1(component)), ' ');
        }

        template <typename Component, typename Context>
        static std::string what(Component const& component, Context const& ctx)
        {
            std::string result = "center(";

            result += boost::lexical_cast<std::string>(
                proto::child_c<0>(argument1(component)));
            result += ")[";

            typedef typename
                spirit::result_of::subject<Component>::type::director
            director;

            result += director::what(spirit::subject(component), ctx);
            result += "]";
            return result;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //  The center alignment directive with padding, is used for generators like
    //  center(padding)[...], where padding is a arbitrary generator
    //  expression. It uses a default value for the generated width (defined
    //  via the BOOST_KARMA_DEFAULT_FIELD_LENGTH constant).
    ///////////////////////////////////////////////////////////////////////////
    struct padding_center_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::subject<Component>::type,
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter, typename Parameter>
        static bool
        generate(Component const& component, OutputIterator& sink,
            Context& ctx, Delimiter const& d, Parameter const& param)
        {
            return detail::center_generate(sink, ctx, d, param,
                subject(component), BOOST_KARMA_DEFAULT_FIELD_LENGTH,
                argument1(component));
        }

        template <typename Component, typename Context>
        static std::string what(Component const& component, Context const& ctx)
        {
            std::string result = "center(";

            typedef typename
                spirit::result_of::argument1<Component>::type::director
            padding;

            result += padding::what(spirit::argument1(component), ctx);
            result += ")[";

            typedef typename
                spirit::result_of::subject<Component>::type::director
            director;

            result += director::what(spirit::subject(component), ctx);
            result += "]";
            return result;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //  The full center alignment directive, is used for generators like
    //  center(width, padding)[...], where width is a integer value to be
    //  used as the field width and padding is a arbitrary generator
    //  expression.
    ///////////////////////////////////////////////////////////////////////////
    struct full_center_aligment
    {
        template <typename Component, typename Context, typename Unused>
        struct attribute
          : traits::attribute_of<
                karma::domain,
                typename result_of::subject<Component>::type,
                Context
            >
        {
        };

        template <typename Component, typename OutputIterator,
            typename Context, typename Delimiter, typename Parameter>
        static bool
        generate(Component const& component, OutputIterator& sink,
            Context& ctx, Delimiter const& d, Parameter const& param)
        {
            return detail::center_generate(sink, ctx, d, param,
                subject(component), proto::child_c<0>(argument1(component)),
                argument2(component));
        }

        template <typename Component, typename Context>
        static std::string what(Component const& component, Context const& ctx)
        {
            std::string result = "center(";

            result += boost::lexical_cast<std::string>(
                proto::child_c<0>(argument1(component)));
            result += ", ";

            typedef typename
                spirit::result_of::argument2<Component>::type::director
            padding;

            result += padding::what(spirit::argument2(component), ctx);
            result += ")[";

            typedef typename
                spirit::result_of::subject<Component>::type::director
            director;

            result += director::what(spirit::subject(component), ctx);
            result += "]";
            return result;
        }
    };

}}} // namespace boost::spirit::karma

#endif


