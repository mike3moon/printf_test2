#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct  tag
{
    int minus;
    int zero;
    int width;
    int precision;
    char    type;
}   tag;

const char	*read_flag(const char *format, tag *ap_tag)
{
	while (*format == '-' || *format == '0')
	{
		if (*format == '-')
			ap_tag->minus = 1;
		else
			ap_tag->zero = 1;
		format++;
	}
	return (format);
}

const char	*read_width(const char *format, tag *ap_tag, va_list ap)
{
	while ((*format >= '0' && *format <= '9') || *format == '*')
	{
		if (*format >= '0' && *format <= '9')
		{
			ap_tag->width *= 10;
			ap_tag->width += (*format - '0');
		}
		if (*format == '*')
		{
			ap_tag->width = va_arg(ap, int);
			if (ap_tag->width < 0)
			{
				ap_tag->minus = 1;
				ap_tag->width *= (-1);
			}
			if (ap_tag->width == 0)
				ap_tag->zero = 1;
		}
		format++;
	}
	return (format);
}

const char	*read_precision(const char *format, tag *ap_tag, va_list ap)
{
	if (*format == '.')
	{
		format++;
		if (*format == '-')
		{
			format++;
			while (*format >= '0' && *format <= '9')
			{
				ap_tag->precision *= 10;
				ap_tag->precision -= (*format - '0');
				format++;
			}
		}
		else if (*format >= '0' && *format <= '9')
		{
			while (*format >= '0' && *format <= '9')
			{
				ap_tag->precision *= 10;
				ap_tag->precision += (*format - '0');
				format++;
			}
		}
		else if (*format == '*')
			ap_tag->precision += va_arg(ap, int);
		format++;
	}
	else
		ap_tag->precision = -1;
	return (format);
}

const char	*read_type(const char *format, tag *ap_tag)
{
	if (*format == 'c')
		ap_tag->type = 'c';
	else if (*format == 's')
		ap_tag->type = 's';
	else if (*format == 'p')
		ap_tag->type = 'p';
	else if (*format == 'd')
		ap_tag->type = 'd';
	else if (*format == 'i')
		ap_tag->type = 'i';
	else if (*format == 'u')
		ap_tag->type = 'u';
	else if (*format == 'x')
		ap_tag->type = 'x';
	else if (*format == 'X')
		ap_tag->type = 'X';
	else if (*format == '%')
		ap_tag->type = '%';
	return (format);
}

const char	*read_tag(const char *format, tag *ap_tag, va_list ap)
{
	format = read_flag(format, ap_tag);
	format = read_width(format, ap_tag, ap);
	format = read_precision(format, ap_tag, ap);
	format = read_type(format, ap_tag);
	return (format);
}

void	init_tag(tag *ap_tag)
{
	ap_tag->minus = 0;
	ap_tag->zero = 0;
	ap_tag->width = 0;
	ap_tag->precision = 0;
}

void	write_char(int c, tag *ap_tag, int *ret)
{
	int	width;

	width = ap_tag->width;
	if (ap_tag->minus == 1)
	{
		write(1, &c, 1);
		(*ret)++;
	}
	while (--width > 0)
	{
		write(1, " ", 1);
		(*ret)++;
	}
	if (ap_tag->minus == 0)
	{
		write(1, &c, 1);
		(*ret)++;
	}
}

int	write_string(char *str, int width, int prec, tag *ap_tag)
{
	int	ret;

	ret = 0;
	if (ap_tag->minus == 1)
	{
		while (*str && prec-- > 0)
		{
			write(1, str++, 1);
			ret++;
		}
	}
	while (width-- > 0)
	{
		write(1, " ", 1);
		ret++;
	}
	if (ap_tag->minus == 0)
	{
		while (*str && prec-- > 0)
		{
			write(1, str++, 1);
			ret++;
		}
	}
	return (ret);
}

void	print_string(char *str, tag *ap_tag, int *ret)
{
	int	width;
	int	prec;

	prec = ap_tag->precision;
	if (prec < 0)
		prec = strlen(str);
	if (prec <= strlen(str))
		width = ap_tag->width - prec;
	else
		width = ap_tag->width - strlen(str);
	*ret += write_string(str, width, prec, ap_tag);
}

const char	*write_ap(const char *format, va_list ap, int *ret)
{
	tag	*ap_tag;

	ap_tag = malloc(sizeof(tag) * 1);
	if (!ap_tag)
		return (0);
	init_tag(ap_tag);
	format++;
	format = read_tag(format, ap_tag, ap);
	if (ap_tag->type == 'c')
		write_char(va_arg(ap, int), ap_tag, ret);
	if (ap_tag->type == 's')
		print_string(va_arg(ap, char *), ap_tag, ret);
    free(ap_tag);
	return (format);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		ret;

	ret = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format == '%')
		{
			format = write_ap(format, ap, &ret);
		}
		else
		{
			write(1, format, 1);
			ret++;
		}
		format++;
	}
	va_end(ap);
	return (0);
}

int main()
{
    ft_printf("[%-2s]", "a");
    return 0;
}
